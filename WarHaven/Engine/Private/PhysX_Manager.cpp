#include "..\Public\PhysX_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPhysX_Manager)

//#define	YJ_DEBUG

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

	

	// Create PhysX
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPVD);

	// Create Cooking
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(PxTolerancesScale()));

	// Crate Material
	// 충돌체 정지 마찰계수, 운동 마찰 계수, 탄성력
	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, -10.f);

	for (_uint i = 0; i < SCENE_END; ++i)
	{
		m_pScenes[i] = nullptr;
	}



	return S_OK;
}


void CPhysX_Manager::Tick()
{
	_float fTimeDelta = fDT(0);

	if (m_pCurScene)
	{
		if (1 == m_pCurScene->getTimestamp() ||
			2 == m_pCurScene->getTimestamp())
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

	// Set Dispatcher
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pScenes[eScene] = m_pPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_pScenes[eScene]->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_pCurScene = m_pScenes[eScene];

	return S_OK;
}

HRESULT CPhysX_Manager::Delete_Scene(Scene eScene)
{
	m_pScenes[eScene]->release();
	m_pScenes[eScene] = nullptr;

	m_pDispatcher->release();
	m_pDispatcher = nullptr;
	return S_OK;
}

HRESULT CPhysX_Manager::Change_Scene(Scene eNextScene, PxVec3 Gravity)
{
	m_pCurScene->release();
	Create_Scene(eNextScene, Gravity);
	m_pCurScene = m_pScenes[eNextScene];
	return S_OK;
}

PxRigidDynamic * CPhysX_Manager::Create_DynamicActor(const PxTransform & Transform, const PxGeometry & Geometry, Scene eScene, const PxReal& Density, const PxVec3 & velocity, PxMaterial* pMaterial)
{
	PxRigidDynamic* pDynamic = nullptr;
	if (pMaterial)
		pDynamic = PxCreateDynamic(*m_pPhysics, Transform, Geometry, *pMaterial, 10.f);
	else
		pDynamic = PxCreateDynamic(*m_pPhysics, Transform, Geometry, *m_pMaterial, 10.f);

	// 저항값
	pDynamic->setAngularDamping(0.5f);
	// 속도
	pDynamic->setLinearVelocity(velocity);

	pDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	PxRigidBodyExt::updateMassAndInertia(*pDynamic, Density);
	m_pCurScene->addActor(*pDynamic);
	//m_pScenes[eScene]->addActor(*pDynamic);
	return pDynamic;
}

PxRigidStatic * CPhysX_Manager::Create_StaticActor(const PxTransform & Transform, const PxGeometry & Geometry, Scene eScene, PxMaterial* pMaterial)
{
	PxRigidStatic* pStatic = nullptr;
	if (pMaterial)
		pStatic = PxCreateStatic(*m_pPhysics, Transform, Geometry, *pMaterial);
	else
		pStatic = PxCreateStatic(*m_pPhysics, Transform, Geometry, *m_pMaterial);

	m_pCurScene->addActor(*pStatic);

	return pStatic;
}


void CPhysX_Manager::Create_ConvexMesh(PxVec3 ** pVertices, _uint iNumVertice, PxConvexMesh ** ppOut)
{
	PxCookingParams params = m_pCooking->getParams();

	// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
	params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;

	// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
	// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
	params.gaussMapLimit = 16;
	m_pCooking->setParams(params);

	// Setup the convex mesh descriptor
	PxConvexMeshDesc Desc;

	// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
	Desc.points.data = *pVertices;
	Desc.points.count = iNumVertice;
	Desc.points.stride = sizeof(PxVec3);
	Desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	*ppOut = m_pCooking->createConvexMesh(Desc, m_pPhysics->getPhysicsInsertionCallback());
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

	Create_ConvexMesh(&pVertices, iNumVerts, ppOut);
	Safe_Delete_Array(pVertices);
}


void CPhysX_Manager::Release()
{


	for (_uint i = 0; i < SCENE_END; ++i)
	{
		Safe_release(m_pScenes[i]);
	}


	Safe_release(m_pDispatcher);

	PX_UNUSED(true);

	Safe_release(m_pPhysics);
	Safe_release(m_pCooking);




	if (m_pPVD)
	{
		PxPvdTransport* transport = m_pPVD->getTransport();
		Safe_release(m_pPVD);
		transport->release();
	}
	Safe_release(m_pFoundation);
}

