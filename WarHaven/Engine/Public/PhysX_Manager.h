#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;


class CPhysX_Manager
	: public PxSimulationEventCallback
{
	DECLARE_SINGLETON(CPhysX_Manager)

public:
	

	enum Scene
	{
		SCENE_CURRENT,
		SCENE_NEXT,
		SCENE_BEFORE,

		SCENE_END
	};

	typedef enum tagColliderShape
	{
		SHAPE_CHEESE,

		SHAPE_END
	}COLSHAPE;

	

private:
	CPhysX_Manager();
	virtual ~CPhysX_Manager();

public:
	PxConvexMesh*	Get_ShapeTemplate(COLSHAPE eShape) { return m_ShapeTemplate[eShape]; }
	void			Set_ShapeSphere(PxSphereGeometry* pGeometry) { m_pSphere = pGeometry; }
	void			Set_ShapeBox(PxBoxGeometry* pGeometry) { m_pBox = pGeometry; }

	void			Begin_PhysScene() { m_bSceneStart = true; }
	void			End_PhysScene() { m_bSceneStart = false; }

	/* Simulation Callback*/
public:
	void onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/)
	{
		//printf("onConstraintBreak\n");
	}
	void onWake(PxActor** /*actors*/, PxU32 /*count*/)
	{
		//printf("onWake\n");
	}
	void onSleep(PxActor** /*actors*/, PxU32 /*count*/)
	{
		//printf("onSleep\n");
	}
	void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
	{
		//printf("onAdvance\n");
	}
	void onContact(const PxContactPairHeader& /*pairHeader*/, const PxContactPair* pairs, PxU32 count);

	
	
public:
	HRESULT	Initialize();
	void	Tick();
	void	Release();


public:
	// Transform 에는 (float3)위치와 (float4)쿼터니온이 들어간다.
	HRESULT			Create_Scene(Scene eScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));
	HRESULT			Delete_Scene(Scene eScene);
	HRESULT			Change_Scene(Scene eNextScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));

	PxRigidDynamic* Create_DynamicActor(const PxTransform& t, const PxGeometry& geometry, Scene eScene, const PxReal& Density
		, _bool bTrigger = false, TRIGGERDESC	tTriggerDesc = TRIGGERDESC()
		, const PxVec3& velocity = PxVec3(0)
		, PxMaterial* pMaterial = nullptr);

	PxRigidStatic*	Create_StaticActor(const PxTransform& t, const PxGeometry& geometry, Scene eScene
		, _bool bTrigger = false, TRIGGERDESC	tTriggerDesc = TRIGGERDESC()
		, PxMaterial* pMaterial = nullptr);
	
	void			Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut);
	void			Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut);
	void			Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut);
	void			Create_Shape(const PxGeometry & Geometry, PxMaterial* pMaterial, PxShape ** ppOut);

public:
	void			Create_PxControllerManager(Scene eScene);
	void			Create_CapsuleController(_float fRadius, _float fHeight, PxController** ppOut);

public: /* Trigger */
	void			Create_Trigger(const TRIGGERDESC& tTriggerDesc, const PxGeometry& eGeometry, PxRigidActor* pActor);
	bool			isTriggerShape(PxShape* shape);

public:
	TRIGGERDESC Find_Trigger(string strName);

	
private:
	map<_hashcode, TRIGGERDESC>	m_mapTrigger;

private:
	PxControllerManager* m_pPxControllerManager = nullptr;

private:
	// Foundation을 생성하는데 필요한 변수
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxFoundation*			m_pFoundation;

	// PxFoundation이 있어야 Physics를 생성할 수 있다.
	PxPhysics*				m_pPhysics;

	// CPU 리소스를 효율적으로 공유할 수 있도록 하기 위해 구현하는 것을 추천
	PxDefaultCpuDispatcher*	m_pDispatcher;

	// MeshCooking을 하기 위해 생성
	PxCooking*				m_pCooking = nullptr;

	// Scene
	//_uint					m_iNumScenes = 0;
	//typedef PxScene*		LPSCENE;
	PxScene*				m_pScenes[SCENE_END];
	PxScene*				m_pCurScene = nullptr;

	// m_pPhysics를 사용해 createMaterial해서 사용
	// 충돌체 마찰력, Dynamic 마찰력, 탄성력을 지정하여 사용
	PxMaterial*				m_pMaterial;

	////Visual Debugger
	PxPvd*					m_pPVD;

	PxRigidDynamic*			m_pTemp = nullptr;

private:
	PxBoxGeometry*			m_pBox = nullptr;
	PxSphereGeometry*		m_pSphere = nullptr;

	PxConvexMesh*			m_ShapeTemplate[SHAPE_END];
	_bool					m_bSceneStart = false;
};

END