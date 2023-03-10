#include "..\Public\PhysX_Manager.h"
#include "GameInstance.h"

#include "GameObject.h"
#include "CUtility_PhysX.h"

IMPLEMENT_SINGLETON(CPhysX_Manager)

#define	YJ_DEBUG
//#define USE_GPU


CPhysX_Manager::CPhysX_Manager()
{
}

CPhysX_Manager::~CPhysX_Manager()
{
	Release();
}

HRESULT CPhysX_Manager::Initialize()
{
	// Create Foundation
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	// Create PVD
#ifdef YJ_DEBUG
	char	strTransport[MIN_STR] = "127.0.0.1";
	m_pPVD = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* Transport = PxDefaultPvdSocketTransportCreate(strTransport, 5425, 10);
	_bool	bPVDConnectionResult = m_pPVD->connect(*Transport, PxPvdInstrumentationFlag::eALL);

	if (!bPVDConnectionResult)
	{
		Call_MsgBox(L"Faiied to connect to PVD!");
	}
#endif // YJ_EDEBUG

#ifdef USE_GPU
	/* GPU */
	if (FAILED(SetUp_PxCudaContextManager()))
		return E_FAIL;
#endif
	// Create PhysX
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPVD);

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(PxTolerancesScale()));
	

	// Crate Material
	// ?浹ü ???? ????????, ? ???? ????, ź????
	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, -10.f);

	for (_uint i = 0; i < SCENE_END; ++i)
	{
		m_pScenes[i] = nullptr;
	}

	m_listAllStatics.clear();
	m_pTerrainStatic = nullptr;

	return S_OK;
}


void CPhysX_Manager::Tick()
{
	_float fTimeDelta = fDT(0);

	if (m_bSceneStart && m_pCurScene)
	{
		if (2 >= m_pCurScene->getTimestamp())
			fTimeDelta = 0.16f;

		m_pCurScene->simulate(fTimeDelta);
		m_pCurScene->fetchResults(true);
	}
}


HRESULT CPhysX_Manager::Create_Scene(Scene eScene, PxVec3 Gravity)
{
	// Set Scene
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = Gravity;

	m_listAllStatics.clear();
	

	/* GPU SETTING */

#ifdef USE_GPU
	m_pDispatcher = PxDefaultCpuDispatcherCreate(4);
#else
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
#endif

	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
#ifdef USE_GPU

	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
	sceneDesc.cudaContextManager = m_pPxCudaContextManager;
	sceneDesc.gpuMaxNumPartitions = 16;

#else
#endif


	//sceneDesc.filterShader = triggersUsingFilterShader;
	//sceneDesc.filterShader = triggersUsingFilterCallback;
	//sceneDesc.filterCallback = &gTriggersFilterCallback;

	m_pScenes[eScene] = m_pPhysics->createScene(sceneDesc);


	PxPvdSceneClient* pvdClient = m_pScenes[eScene]->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_pCurScene = m_pScenes[eScene];

	Create_PxControllerManager(eScene);

	return S_OK;
}

HRESULT CPhysX_Manager::Delete_Scene(Scene eScene)
{
	Safe_release(m_pPxControllerManager);

	m_pScenes[eScene]->release();
	m_pScenes[eScene] = nullptr;

	m_pDispatcher->release();
	m_pDispatcher = nullptr;

	m_listAllStatics.clear();

	return S_OK;
}

HRESULT CPhysX_Manager::Change_Scene(Scene eNextScene, PxVec3 Gravity)
{
	m_pCurScene->release();
	Create_Scene(eNextScene, Gravity);
	m_pCurScene = m_pScenes[eNextScene];
	return S_OK;
}

PxRigidDynamic * CPhysX_Manager::Create_DynamicActor(const PxTransform & Transform, const PxGeometry & Geometry, Scene eScene, const PxReal& Density
	,const PxVec3 & velocity, PxMaterial* pMaterial)
{
	PxRigidDynamic* pDynamic = nullptr;
	if (pMaterial)
		pDynamic = PxCreateDynamic(*m_pPhysics, Transform, Geometry, *pMaterial, 10.f);
	else
		pDynamic = PxCreateDynamic(*m_pPhysics, Transform, Geometry, *m_pMaterial, 10.f);

	// ???װ?
	pDynamic->setAngularDamping(0.5f);
	// ?ӵ?
	pDynamic->setLinearVelocity(velocity);

	pDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	//PxShapeFlag::eSCENE_QUERY_SHAPE;


	PxRigidBodyExt::updateMassAndInertia(*pDynamic, Density);
	m_pCurScene->addActor(*pDynamic);
	//m_pScenes[eScene]->addActor(*pDynamic);
	return pDynamic;
}

PxRigidStatic * CPhysX_Manager::Create_StaticActor(const PxTransform & Transform, const PxGeometry & Geometry, Scene eScene
	, PxMaterial* pMaterial)
{
	PxRigidStatic* pStatic = nullptr;
	if (pMaterial)
		pStatic = PxCreateStatic(*m_pPhysics, Transform, Geometry, *pMaterial);
	else
		pStatic = PxCreateStatic(*m_pPhysics, Transform, Geometry, *m_pMaterial);

	if (!pStatic)
	{
		Call_MsgBox(L"Failed to PxCreateStatic : CPhysX_Manager");
		return nullptr;
	}

	m_pCurScene->addActor(*pStatic);
	m_listAllStatics.push_back(pStatic);

	if (Geometry.getType() == PxGeometryType::eTRIANGLEMESH)
		m_pTerrainStatic = pStatic;

	return pStatic;
}

void CPhysX_Manager::Erase_Static(PxRigidStatic* pStatic)
{
	for (auto iter = m_listAllStatics.begin(); iter != m_listAllStatics.end(); ++iter)
	{
		if ((*iter) == pStatic)
		{
			iter = m_listAllStatics.erase(iter);
			return;
		}
	}
}


void CPhysX_Manager::Create_ConvexMesh(_float3* pVerticesPos, _uint iNumVertices, void* pIndices, _uint iNumPrimitive, PxConvexMesh ** ppOut)
{
	PxVec3* pPxVerticesPos = new PxVec3[iNumVertices];
	memcpy(pPxVerticesPos, pVerticesPos, sizeof(PxVec3) * iNumVertices);

	PxCookingParams params = m_pCooking->getParams();

	// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
	params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;

#ifdef USE_GPU
	params.buildGPUData = true;
#endif

	// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
	// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
	params.gaussMapLimit = 16;
	m_pCooking->setParams(params);

	// Setup the convex mesh descriptor
	PxConvexMeshDesc Desc;

	// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
	Desc.points.data = pPxVerticesPos;
	Desc.points.count = iNumVertices;
	Desc.points.stride = sizeof(PxVec3);
	Desc.vertexLimit = 12;

	/*Desc.indices.count = iNumPrimitive;
	Desc.indices.data = pIndices;
	Desc.indices.stride = sizeof(FACEINDICES32);

	Desc.*/

	Desc.flags |= PxConvexFlag::eCOMPUTE_CONVEX;
	if (iNumVertices < 10)
	{
		SAFE_DELETE_ARRAY(pPxVerticesPos);
		return;
		Desc.flags |= PxConvexFlag::ePLANE_SHIFTING;

		Desc.vertexLimit = 4;

	}

	if (iNumVertices > 2000)
	{
		/*if (iNumVertices > 14000)
		{
			Call_MsgBox(L"???? ?ʹ? ???Ƽ? Convex ??????, ?ǳ? ?ٰ???.");
			SAFE_DELETE_ARRAY(pPxVerticesPos);
			return;
		}*/

		Desc.vertexLimit = 40;
		Desc.quantizedCount = iNumVertices / 2;
		Desc.flags |= PxConvexFlag::eQUANTIZE_INPUT;
	}

	

	if (!Desc.isValid())
	{
		assert(0);
	}

	PxConvexMeshCookingResult::Enum eResult;
	*ppOut = m_pCooking->createConvexMesh(Desc, m_pPhysics->getPhysicsInsertionCallback(), &eResult);

	if (eResult != PxConvexMeshCookingResult::Enum::eSUCCESS)
		assert(0);

	SAFE_DELETE_ARRAY(pPxVerticesPos);

}

void CPhysX_Manager::Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial ** ppOut)
{
	*ppOut = m_pPhysics->createMaterial(fStaticFriction, fDynamicFriction, fRestitution);
}

void CPhysX_Manager::Create_Shape(const PxGeometry & Geometry, PxMaterial* pMaterial, PxShape ** ppOut)
{
	if (pMaterial)
		*ppOut = m_pPhysics->createShape(Geometry, *pMaterial);
	else
		*ppOut = m_pPhysics->createShape(Geometry, *m_pMaterial);
}

void CPhysX_Manager::Create_CapsuleController(_float fRadius, _float fHeight, PxController** ppOut, PxUserControllerHitReport* pUserData)
{
	/*PxBoxControllerDesc		tBCT;
	tBCT.halfForwardExtent = fRadius;
	tBCT.halfSideExtent = fRadius;
	tBCT.halfHeight = fHeight;
	tBCT.material = m_pMaterial;
	tBCT.reportCallback = pUserData;

	*ppOut = m_pPxControllerManager->createController(tBCT);*/

	PxCapsuleControllerDesc	tCCT;

	tCCT.radius = fRadius;
	tCCT.height = fHeight;
	tCCT.material = m_pMaterial;
	tCCT.position = PxExtendedVec3(0.f, 0.f, 0.f);

	//???? ???̱??? ?ö??? ?? ?ִ???
	tCCT.climbingMode = PxCapsuleClimbingMode::eEASY;
	tCCT.stepOffset = 0.15f;
	tCCT.contactOffset = 0.1f;

	//?????? ???????????? ??? ?? ??
	tCCT.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	tCCT.slopeLimit = cosf(ToRadian(50.f));

	//tCCT.invisibleWallHeight = 0.6f;
	if (!tCCT.isValid())
	{
		Call_MsgBox(L"Failed to Create_CapsuleController");
	}
	//tCCT.maxJumpHeight = 20.f; // default JumpHeight

	tCCT.reportCallback = pUserData;

	tCCT.userData = ppOut;
	*ppOut = m_pPxControllerManager->createController(tCCT);
}

void CPhysX_Manager::Create_TriangleMesh(_float3* pVerticesPos, _uint iNumVertices, _uint iNumPrimitive, PxTriangleMesh** ppOut)
{
	PxVec3* pPxVerticesPos = new PxVec3[iNumVertices];
	memcpy(pPxVerticesPos, pVerticesPos, sizeof(PxVec3) * iNumVertices);

	//?ͷ????? ???? position??

	PxTriangleMeshDesc	tMeshDesc;
	tMeshDesc.points.count = iNumVertices;
	tMeshDesc.points.stride = sizeof(PxVec3);
	tMeshDesc.points.data = pPxVerticesPos;

	tMeshDesc.triangles.count = iNumPrimitive;
	tMeshDesc.triangles.stride = sizeof(FACEINDICES32);


#ifdef USE_GPU
	PxCookingParams cookingParam = m_pCooking->getParams();
	cookingParam.buildGPUData = true;
	m_pCooking->setParams(cookingParam);
#endif
	PxDefaultMemoryOutputStream	writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	_bool	bStatus = m_pCooking->cookTriangleMesh(tMeshDesc, writeBuffer, &result);

	if (!bStatus)
		return;

	PxDefaultMemoryInputData	readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	*ppOut = m_pPhysics->createTriangleMesh(readBuffer);

	SAFE_DELETE_ARRAY(pPxVerticesPos);
}

_bool CPhysX_Manager::Shoot_Ray(_float4 vStartPos, _float4 vRayDir, _float4* pOutPos)
{
	


	return _bool();
}

HRESULT CPhysX_Manager::SetUp_PxCudaContextManager()
{
	PxCudaContextManagerDesc cudaContextManagerDesc;
	cudaContextManagerDesc.interopMode = PxCudaInteropMode::D3D11_INTEROP;
	cudaContextManagerDesc.graphicsDevice = PDEVICE;
	m_pPxCudaContextManager = PxCreateCudaContextManager(*m_pFoundation, cudaContextManagerDesc, PxGetProfilerCallback());

	if (!m_pPxCudaContextManager)
		return E_FAIL;

	if (!m_pPxCudaContextManager->contextIsValid())
	{
		m_pPxCudaContextManager->release();
		m_pPxCudaContextManager = nullptr;
		return E_FAIL;
	}

	return S_OK;
}



void CPhysX_Manager::Create_PxControllerManager(Scene eScene)
{
	if (!m_pScenes[eScene])
		return;

	if (m_pPxControllerManager)
		return;

	m_pPxControllerManager = PxCreateControllerManager(*m_pScenes[eScene]);
}



void CPhysX_Manager::Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh ** ppOut)
{
	_uint	iNumVerts = 32;
	PxVec3* pVertices = new PxVec3[iNumVerts];

	// Below
	for (PxU32 i = 0; i < 16; i++)
	{
		_vector vPostion = { fRadiusBelow, 0.f, 0.f, 1.f };

		_matrix RotateMatrix = XMMatrixRotationAxis(_vector{ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(360.f / 16 * i));
		vPostion = XMVector3TransformCoord(vPostion, RotateMatrix);
		pVertices[i] = PxVec3(XMVectorGetX(vPostion), -fHight * 0.5f, XMVectorGetZ(vPostion));
	}

	// Upper
	for (PxU32 i = 16; i < 32; i++)
	{
		_vector vPostion = { fRadiusUpper, 0.f, 0.f, 1.f };

		_matrix RotateMatrix = XMMatrixRotationAxis(_vector{ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(360.f / 16 * i));
		vPostion = XMVector3TransformCoord(vPostion, RotateMatrix);
		pVertices[i] = PxVec3(XMVectorGetX(vPostion), fHight * 0.5f, XMVectorGetZ(vPostion));
	}

	//Create_ConvexMesh(&pVertices, iNumVerts, ppOut);
	Safe_Delete_Array(pVertices);
}


void CPhysX_Manager::Release()
{
	PX_UNUSED(true);

	Safe_release(m_pPxControllerManager);
	Safe_release(m_pPxCudaContextManager);

	Safe_release(m_pDispatcher);

	for (_uint i = 0; i < SCENE_END; ++i)
	{
		Safe_release(m_pScenes[i]);
	}

	Safe_release(m_pPhysics);
	Safe_release(m_pCooking);


	if (m_pPVD)
	{
		PxPvdTransport* transport = m_pPVD->getTransport();
		Safe_release(m_pPVD);
		transport->release();
	}


	
	
	Safe_release(m_pFoundation);
	m_listAllStatics.clear();

}

_bool CPhysX_Manager::Shoot_RaytoStaticActors(_float4* pOutPos, _float* pMinDist, _float4 vStartPos, _float4 vStartDir, _float fMaxDistance)
{
	_float fMinDist = 9999.f;
	_float4 vFinalHitPos = vStartPos + vStartDir.Normalize() * fMaxDistance;

	for (auto& elem : m_listAllStatics)
	{
		if (!elem)
			continue;

		//static?̶? ?????? ?̳༮???? ray ???? ??ġ ????
		PxRaycastHit hitInfo;
		PxShape* pCurShape = nullptr;
		elem->getShapes(&pCurShape, sizeof(PxShape));

		if (!pCurShape)
			continue;

		if (pCurShape->getGeometryType() == PxGeometryType::eBOX)
		{
			/* ????ü */
			_float4 vCenterPos = CUtility_PhysX::To_Vector(elem->getGlobalPose().p);
			_float fRadius = sqrtf(pow(pCurShape->getGeometry().box().halfExtents.x, 2.f) + pow(pCurShape->getGeometry().box().halfExtents.y, 2.f) + pow(pCurShape->getGeometry().box().halfExtents.z, 2.f));

			if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(vCenterPos.XMLoad(), fRadius))
			{
				continue;
			}

			if (((vCenterPos - vStartPos).Length() - fRadius) >= fMaxDistance)
				continue;
		}


		PxU32 hitCount = PxGeometryQuery::raycast(
			CUtility_PhysX::To_PxVec3(vStartPos),
			CUtility_PhysX::To_PxVec3(vStartDir),
			pCurShape->getGeometry().any(),
			elem->getGlobalPose(),
			fMaxDistance, PxHitFlag::ePOSITION, 1, &hitInfo
		);

		if (hitCount > 0)
		{
			_float4 vHitPos = CUtility_PhysX::To_Vector(hitInfo.position);
			_float fLength = (vHitPos - vStartPos).Length();

			if (fLength < fMinDist)
			{
				fMinDist = fLength;
				vFinalHitPos = vHitPos;
			}

		}
	}

	if (fMinDist == 9999.f)
	{
		if (pOutPos)
			*pOutPos = vFinalHitPos;

		return false;
	}

	/* GROUP_PLAYER ?˻? */

	if (pOutPos)
		*pOutPos = vFinalHitPos;
	if (pMinDist)
		*pMinDist = fMinDist;

	return true;
}

_bool CPhysX_Manager::Shoot_RaytoControllers(list<PxController*>& listControllers, _float fMinDist, _float4* pOutPos, _float4 vStartPos, _float4 vStartDir, _float fMaxDistance)
{
	_float4 vFinalHitPos = *pOutPos;


	for (auto& elem : listControllers)
	{
		//static?̶? ?????? ?̳༮???? ray ???? ??ġ ????
		PxRaycastHit hitInfo;
		PxShape* pCurShape = nullptr;
		elem->getActor()->getShapes(&pCurShape, sizeof(PxShape));

		PxU32 hitCount = PxGeometryQuery::raycast(
			CUtility_PhysX::To_PxVec3(vStartPos),
			CUtility_PhysX::To_PxVec3(vStartDir),
			pCurShape->getGeometry().any(),
			elem->getActor()->getGlobalPose(),
			fMaxDistance, PxHitFlag::ePOSITION, 1, &hitInfo
		);

		if (hitCount > 0)
		{
			_float4 vHitPos = CUtility_PhysX::To_Vector(hitInfo.position);
			_float fLength = (vHitPos - vStartPos).Length();

			if (fLength < fMinDist)
			{
				fMinDist = fLength;
				vFinalHitPos = vHitPos;
			}

		}
	}

	/* GROUP_PLAYER ?˻? */

	*pOutPos = vFinalHitPos;
	return true;
}

_bool CPhysX_Manager::Shoot_RaytoTerrain(_float4* pOutPos, _float* pOutDist, _float4 vStartPos, _float4 vStartDir)
{
	if (!m_pTerrainStatic)
		return false;

	vStartPos.y += 0.5f;

	PxRaycastHit hitInfo;
	PxShape* pCurShape = nullptr;
	m_pTerrainStatic->getShapes(&pCurShape, sizeof(PxShape));

	PxU32 hitCount = PxGeometryQuery::raycast(
		CUtility_PhysX::To_PxVec3(vStartPos),
		CUtility_PhysX::To_PxVec3(vStartDir),
		pCurShape->getGeometry().any(),
		m_pTerrainStatic->getGlobalPose(),
		1000.f, PxHitFlag::ePOSITION, 1, &hitInfo
	);

	if (hitCount > 0)
	{
		_float4 vHitPos = CUtility_PhysX::To_Vector(hitInfo.position);
		_float fLength = (vHitPos - vStartPos).Length();

		*pOutDist = fLength - 0.5f;
		*pOutPos = vHitPos;
		return true;
	}


	return false;
}

