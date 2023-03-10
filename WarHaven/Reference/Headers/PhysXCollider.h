#pragma once
#include "Component.h"

BEGIN(Engine)
class CTransform;
class CMesh;

class ENGINE_DLL CPhysXCollider 
	: public CComponent
{
	DECLARE_PROTOTYPE(CPhysXCollider);

public:
	enum class COLLIDERTYPE { DYNAMIC, STATIC, DECORATION, ZONE, YFIXED_DYNAMIC, CHARACTER, TYPE_END};
	enum class COLLIDERSHAPE { SPHERE, BOX, CYLINDER, CONVECMESH, TRIANGLEMESH, SHAPE_END };

	typedef struct PhysXColliderDesc
	{
		PhysXColliderDesc() 
		{
			ZeroMemory(this, sizeof(PhysXColliderDesc));
		}

		PxTriangleMesh* pTriangleMesh = nullptr;
		PxConvexMesh*		pConvecMesh = nullptr;
		PxMaterial*			pMaterial = nullptr;
		COLLIDERTYPE		eType = COLLIDERTYPE::TYPE_END;
		_float4				vPosition;
		_float4				vAngles;
		COLLIDERSHAPE		eShape;
		_float4				vScale;
		float				fDensity;

		/*Quaternion ??????*/
		_float4				vQuat = ZERO_VECTOR;

	}PHYSXCOLLIDERDESC;

public:
	CPhysXCollider(_uint iGroupID);
	CPhysXCollider(const CPhysXCollider& rhs);
	virtual ~CPhysXCollider();


public:
	static CPhysXCollider* Create(_uint iGroupID, const PHYSXCOLLIDERDESC& tPhysXColliderDesc);
	static CPhysXCollider* Create(_uint iGroupID, CMesh*	pMesh, CTransform* pWorldTransform);
	static CPhysXCollider* Create_Convex(_uint iGroupID, CMesh*	pMesh, CTransform* pWorldTransform);

public:
	void	Regist_CollisionCallBack();

public:
	_vector	Get_Position();
	_vector	Get_Velocity();
	_float	Get_Mess();
	_vector	Get_AngularVelocity();
	_vector	Get_LinearVelocity();


	HRESULT Set_Position(_vector _vPos, _vector _vQuaternion);
	HRESULT Set_Position(_vector _vPos);
	HRESULT Rotate(_vector _vQuaternion);

	void	Set_MaxLinearVelocity(_vector vMaxVelocity) { m_vMaxVelocity = vMaxVelocity; }
	void	Set_Scale(_float4 vScale);
	void	Set_Scale(_vector vScale);
	void	Set_ActorFlag(PxActorFlag::Enum eFlag, _bool bState);

	void	Set_PickState(_bool bState) { m_bPickState = bState; }
	void	Set_Pickable(_bool bState) { m_bPickable = bState; }
	void	Set_YFixed() { m_bYFixed = true; }

	void	Set_ConvexMesh(_uint iIndex, PxConvexMesh* pMesh) { m_ConvexMeshes[iIndex] = pMesh; }

	_bool	Is_Pickable() { return m_bPickable; }
	_bool	Is_Picked() { return m_bPickState; }
	_bool	Is_YFixed() { return m_bYFixed; }

	void	Delete_Collider();
	void	Create_Collider();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;
	virtual void	Release() override;

	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

public:
	void	Synchronize_Transform(CTransform* pTransform);
	void	Synchronize_Transform_Position(CTransform* pTransform);
	void	Synchronize_Transform_Rotation(CTransform* pTransform);

	void	Synchronize_Collider(CTransform* pTransform);

public:
	void	PutToSleep();
	void	WakeUp();
	void	Attach_Shape(PxShape* pShape);
	void	Detach_Shape(PxShape* pShape);


public:
	HRESULT Add_Force(_vector _vForce);
	HRESULT Clear_Force();
	HRESULT Clear_Velocity();
	HRESULT Add_LinearVelocityResistance(_vector fResistanceRate);

protected:
	// ?ִ? ?ӵ??? XZ,Y ?? ?????? ????. XZ?? ????ִ? ???? X?? ?ִ? ???? ?????Ѵ?.
	_vector					m_vMaxVelocity;

protected:
	PHYSXCOLLIDERDESC		m_ColliderDesc;
	PxRigidDynamic*			m_pRigidDynamic = nullptr;
	PxRigidStatic*			m_pRigidStatic = nullptr;

	PxConvexMesh*			m_ConvexMeshes[16];

	_bool					m_bPickState = false;
	_bool					m_bPickable = true;
	_bool					m_bYFixed = false;

protected:
	void		CreatePhysActor(PHYSXCOLLIDERDESC PhysXColliderDesc);
	void		Create_DynamicActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh = nullptr);
	void		Create_StaticActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh = nullptr);

	HRESULT		SetUp_StaticMeshActor(CMesh* pMesh, CTransform* pWorldTransform);
	HRESULT		SetUp_StaticConvexActor(CMesh* pMesh, CTransform* pWorldTransform);

};

END