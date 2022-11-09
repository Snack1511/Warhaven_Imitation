#include "..\Public\PhysX_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPhysX_Manager)


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
	char	strTransport[MIN_STR] = "127.0.0.1";
	m_pPVD = PxCreatePvd(*m_pFoundation);
	PxPvdTransport*	Transport = PxDefaultPvdSocketTransportCreate(strTransport, 5425, 10);
	_bool	bPVDConnectionResult = m_pPVD->connect(*Transport, PxPvdInstrumentationFlag::eALL);
	if (!bPVDConnectionResult)
	{
		Call_MsgBox(L"Faiied to connect to PVD!");
	}
	else
	{
		Call_MsgBox(L"sex!");

	}

	// Create PhysX
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPVD);

	// Create Cooking
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(PxTolerancesScale()));

	// Crate Material
	// 충돌체 정지 마찰계수, 운동 마찰 계수, 탄성력
	//m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, -10.f);


	/*if (nullptr != m_pScenes)
		__debugbreak();*/

	//m_iNumScenes = iNumLevels;
	//m_pScenes = new LPSCENE[iNumLevels];

	 //Crate Scene
	Create_Scene(SCENE_CURRENT);
	m_pCurScene = m_pScenes[SCENE_CURRENT];



	// Create Plane
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *m_pMaterial);
	m_pScenes[SCENE_CURRENT]->addActor(*groundPlane);

	/*PxReal stackZ = 0.f;
	for (PxU32 i = 0; i<3; i++)
		Create_Stack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);*/

	Create_StaticActor(PxTransform(PxVec3(0.f, 10.f, 0.f)), PxBoxGeometry(5.f, 5.f, 5.f), SCENE_CURRENT);
	Create_DynamicActor(PxTransform(PxVec3(0.f, 0.f, 0.f)), PxBoxGeometry(5.f, 5.f, 5.f), SCENE_CURRENT, 10.f, PxVec3(0.f,0.f,0.f));



	return S_OK;
}


void CPhysX_Manager::Tick()
{
	_float fTimeDelta = fDT(0);

	if (m_bSceneStart)
	{
		if (1 == m_pCurScene->getTimestamp() ||
			2 == m_pCurScene->getTimestamp())
			fTimeDelta = 0.16f;

		m_pCurScene->simulate(fTimeDelta);
		m_pCurScene->fetchResults(true);
	}

	//PxTransform camera = {0.f, 0.f, 0.f};

	//USEGAMEINSTANCE;
	//if (KEY_DOWN('K'))
	//{
	//	//m_pTemp = CreateDynamicActor(camera, PxSphereGeometry(3.0f), SCENE_FIRST, 10.f, camera.rotate(PxVec3(0, 0, -1)) * 200);



	//	PxConvexMesh* pConvexMesh = nullptr;
	//	Create_CylinderMesh(3.f, 1.f, 4.f, &pConvexMesh);

	//	m_pTemp = Create_DynamicActor(camera, PxConvexMeshGeometry(pConvexMesh), SCENE_CURRENT, 10.f, camera.rotate(PxVec3(0, 0, -1)) * 200);
	//}

	//if (KEY_DOWN('L') && m_pTemp)
	//{
	//	m_pTemp->setGlobalPose(PxTransform(0.f, 0.f, 0.f));
	//	m_pTemp->getGlobalPose();
	//}

	//if (m_pTemp)
	//{
	//	PxVec3 TempPos = m_pTemp->getGlobalPose().p;
	//	_float3 vTempPos = { TempPos.x, TempPos.y ,TempPos.z };
	//}


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

	//PxSceneDesc	SceneDesc(m_pPhysics->getTolerancesScale());

	//// 중력
	//SceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	//SceneDesc.cpuDispatcher = m_pDispatcher;

	//// 시뮬레이션 중 출돌 관련 이벤트 함수가 정의된 클래스를 주면 된다
	////SceneDesc.simulationEventCallback = this;

	//// 시뮬레이션 중 사용할 필터 함수를 주면 된다.
	////SceneDesc.filterShader = FilterShader;

	//m_pScene = m_pPhysics->createScene(SceneDesc);

	//// 해당 Flag를 켜주면 Kineamtic Actor 의 충돌 검사가 가능하다
	//// 기본으로는 이벤트가 날라오지 않는다.

	////m_pScene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_PAIRS, true);
	////m_pScene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS, true);

	//// PVD
	//PxPvdSceneClient* pPvdClient = m_pScene->getScenePvdClient();
	//if (pPvdClient)
	//{
	//	pPvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	//	pPvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	//	pPvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	//}

	//// 바닥 만들기
	//// 충돌 재질을 생성한다.
	//// 각 파라미터는 마찰력과 탄성에 관여한다.
	//// 정지마찰, 운동 마찰
	//PxMaterial*	pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	//PxRigidStatic*	pGroundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *pMaterial);

	//m_pScene->addActor(*pGroundPlane);

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
	//m_pScenes[eScene]->addActor(*pStatic);
	return pStatic;
}

//void CPhysX_Manager::Create_Stack(const PxTransform & t, PxU32 size, PxReal halfExtent)
//{
//	PxShape* shape = m_pPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_pMaterial);
//	for (PxU32 i = 0; i<size; i++)
//
//	{
//		for (PxU32 j = 0; j<size - i; j++)
//		{
//			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
//			PxRigidDynamic* body = m_pPhysics->createRigidDynamic(t.transform(localTm));
//			//PxRigidStatic* body = m_pPhysics->createRigidStatic(t.transform(localTm));
//			body->attachShape(*shape);
//			// 해당 메시의 관성을 넣어줌
//			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
//			m_pCurScene->addActor(*body);
//		}
//	}
//	shape->release();
//}

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
		//pVertices[i] = PxVec3(fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f));

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


//void CPhysX_Manager::Create_CheeseShape()
//{
//
//}

void CPhysX_Manager::Release()
{
	PX_UNUSED(true);
	//for (auto& elem : m_pScenes)
	//{
	//	elem->release();
	//}

	//m_pCurScene->release();
	//m_pDispatcher->release();

	if (m_pPhysics)
		m_pPhysics->release();

	if (m_pCooking)
		m_pCooking->release();

	if (m_pPVD)
	{
		PxPvdTransport* transport = m_pPVD->getTransport();
		m_pPVD->release();
		m_pPVD = nullptr;
		transport->release();
	}
	m_pFoundation->release();
}


//const PxU32 numVerts = 64;
//PxVec3* vertices = new PxVec3[numVerts];
//
//// Prepare random verts
//for (PxU32 i = 0; i < numVerts; i++)
//{
//	vertices[i] = PxVec3(fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f));
//}
//
//PxCookingParams params = m_pCooking->getParams();
//
//// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
//params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;
//
//// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
//// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
//params.gaussMapLimit = 16;
//m_pCooking->setParams(params);
//
//// Setup the convex mesh descriptor
//PxConvexMeshDesc Desc;
//
//// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
//Desc.points.data = vertices;
//Desc.points.count = numVerts;
//Desc.points.stride = sizeof(PxVec3);
//Desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
//
//PxU32 meshSize = 0;
//PxConvexMesh* convex = NULL;
//
//convex = m_pCooking->createConvexMesh(Desc, m_pPhysics->getPhysicsInsertionCallback());
//
//delete[] vertices;