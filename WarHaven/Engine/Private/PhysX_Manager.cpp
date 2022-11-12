#include "..\Public\PhysX_Manager.h"
#include "GameInstance.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(CPhysX_Manager)

//#define	YJ_DEBUG


class TriggersFilterCallback : public PxSimulationFilterCallback
{
	virtual		PxFilterFlags	pairFound(PxU32 /*pairID*/,
		PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/, const PxActor* /*a0*/, const PxShape* s0,
		PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/, const PxActor* /*a1*/, const PxShape* s1,
		PxPairFlags& pairFlags)
	{
		//		printf("pairFound\n");

		if (s0->userData || s1->userData)	// See createTriggerShape() function
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		}
		else
			pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		return PxFilterFlags();
	}

	virtual		void			pairLost(PxU32 /*pairID*/,
		PxFilterObjectAttributes /*attributes0*/,
		PxFilterData /*filterData0*/,
		PxFilterObjectAttributes /*attributes1*/,
		PxFilterData /*filterData1*/,
		bool /*objectRemoved*/)
	{
		//		printf("pairLost\n");
	}

	virtual		bool			statusChange(PxU32& /*pairID*/, PxPairFlags& /*pairFlags*/, PxFilterFlags& /*filterFlags*/)
	{
		//		printf("statusChange\n");
		return false;
	}
}gTriggersFilterCallback;

static PxFilterFlags triggersUsingFilterCallback(PxFilterObjectAttributes, PxFilterData, PxFilterObjectAttributes, PxFilterData, PxPairFlags& pairFlags, const void*, PxU32)
{
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	return PxFilterFlag::eCALLBACK;
}



CPhysX_Manager::CPhysX_Manager()
{
}

CPhysX_Manager::~CPhysX_Manager()
{
	Release();
}

void CPhysX_Manager::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count)
{
	while (count--)
	{
		const PxContactPair& CurrentPair = *pairs++;

		//둘다 트리거일 때만 진행하게
		if (!(isTriggerShape(CurrentPair.shapes[0]) && isTriggerShape(CurrentPair.shapes[1])))
			continue;

		string strfirstName = CurrentPair.shapes[0]->getName();
		string strSecondName = CurrentPair.shapes[1]->getName();

		const TRIGGERDESC& tFirst = Find_Trigger(strfirstName);
		if (!tFirst.pGameObject)
			continue;
		const TRIGGERDESC& tSecond = Find_Trigger(strSecondName);
		if (!tSecond.pGameObject)
			continue;

		//Collision Enter
		if (CurrentPair.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			tFirst.pGameObject->CallBack_CollisionEnter(tSecond.pGameObject,	tFirst.iColType, tSecond.iColType);
			tSecond.pGameObject->CallBack_CollisionEnter(tFirst.pGameObject,		tSecond.iColType, tFirst.iColType);
		}
		//Collision Exit
		else if (CurrentPair.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			tFirst.pGameObject->CallBack_CollisionExit(tSecond.pGameObject, tSecond.iColType);
			tSecond.pGameObject->CallBack_CollisionExit(tFirst.pGameObject, tFirst.iColType);
		}
		else
		{
			tFirst.pGameObject->CallBack_CollisionStay(tSecond.pGameObject, tSecond.iColType);
			tSecond.pGameObject->CallBack_CollisionStay(tFirst.pGameObject, tFirst.iColType);
		}
	}


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

	if (m_bSceneStart && m_pCurScene)
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
	//sceneDesc.flags = PxSceneFlag::eENABLE_GPU_DYNAMICS;
	// Set Dispatcher
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = triggersUsingFilterCallback;
	sceneDesc.filterCallback = &gTriggersFilterCallback;

	sceneDesc.simulationEventCallback = this;


	m_pScenes[eScene] = m_pPhysics->createScene(sceneDesc);
	//m_pScenes[eScene]->setFlag(PxSceneFlag::eENABLE_GPU_DYNAMICS, true);

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

PxRigidDynamic * CPhysX_Manager::Create_DynamicActor(const PxTransform & Transform, const PxGeometry & Geometry, Scene eScene, const PxReal& Density
	, _bool bTrigger, TRIGGERDESC tTriggerDesc
	,const PxVec3 & velocity, PxMaterial* pMaterial)
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
	//PxShapeFlag::eSCENE_QUERY_SHAPE;

	/* Trigger */
	if (bTrigger)
	{
		Create_Trigger(tTriggerDesc, Geometry, pDynamic);
	}


	PxRigidBodyExt::updateMassAndInertia(*pDynamic, Density);
	m_pCurScene->addActor(*pDynamic);
	//m_pScenes[eScene]->addActor(*pDynamic);
	return pDynamic;
}

PxRigidStatic * CPhysX_Manager::Create_StaticActor(const PxTransform & Transform, const PxGeometry & Geometry, Scene eScene
	, _bool bTrigger, TRIGGERDESC tTriggerDesc
	, PxMaterial* pMaterial)
{
	PxRigidStatic* pStatic = nullptr;
	if (pMaterial)
		pStatic = PxCreateStatic(*m_pPhysics, Transform, Geometry, *pMaterial);
	else
		pStatic = PxCreateStatic(*m_pPhysics, Transform, Geometry, *m_pMaterial);

	if (bTrigger)
	{
		Create_Trigger(tTriggerDesc, Geometry, pStatic);
	}


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

void CPhysX_Manager::Create_CapsuleController(_float fRadius, _float fHeight, PxController** ppOut)
{
	PxCapsuleControllerDesc	tCCD;
	tCCD.radius = fRadius;
	tCCD.height = fHeight;
	tCCD.material = m_pMaterial;
	tCCD.position = PxExtendedVec3(0.f, 10.f, 0.f);

	//어느 높이까지 올라갈 수 있는지
	tCCD.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	tCCD.stepOffset = 0.2f;
	tCCD.contactOffset = 0.02f;

	//경사진 슬로프만나면 어떻게 할 지
	tCCD.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	tCCD.slopeLimit = cosf(ToRadian(45.f));\
	//tCCD.maxJumpHeight = 20.f; // default JumpHeight

	*ppOut = m_pPxControllerManager->createController(tCCD);

}

void CPhysX_Manager::Create_Trigger(const TRIGGERDESC& tTriggerDesc, const PxGeometry& eGeometry, PxRigidActor* pActor)
{
	if (Find_Trigger(tTriggerDesc.strName).pGameObject)
	{
		Call_MsgBox(L"GameObject is already exist on CPhysX_Manager / (이름 겹침)");
		return;
	}

	PxShape* TriggerShape = nullptr;

	//PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eTRIGGER_SHAPE;

	TriggerShape = m_pPhysics->createShape(eGeometry, *m_pMaterial, true);
	TriggerShape->userData = TriggerShape;	// Arbitrary rule: it's a trigger if non null
	TriggerShape->setName(tTriggerDesc.strName.c_str());

	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));
	tTransform.p.x = tTriggerDesc.vOffsetPos.x;
	tTransform.p.y = tTriggerDesc.vOffsetPos.y;
	tTransform.p.z = tTriggerDesc.vOffsetPos.z;

	TriggerShape->setLocalPose(tTransform);

	if (!(pActor->attachShape(*TriggerShape)))
	{
		Call_MsgBox(L"Failed to AttachShape : Create_Trigger");
	}

	m_mapTrigger.emplace(make_pair(Convert_ToHash(tTriggerDesc.strName), tTriggerDesc));
}

bool CPhysX_Manager::isTriggerShape(PxShape* shape)
{
	if (shape->userData)
		return true;

	return false;
}

void CPhysX_Manager::Create_PxControllerManager(Scene eScene)
{
	if (!m_pScenes[eScene])
		return;

	if (m_pPxControllerManager)
		return;

	m_pPxControllerManager = PxCreateControllerManager(*m_pScenes[eScene]);
}

TRIGGERDESC CPhysX_Manager::Find_Trigger(string strName)
{
	auto iter = m_mapTrigger.find(Convert_ToHash(strName));

	if (m_mapTrigger.end() == iter)
		return TRIGGERDESC();

	return iter->second;
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
	Safe_release(m_pPxControllerManager);

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

