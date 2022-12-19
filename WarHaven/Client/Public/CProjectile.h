#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)
class CUnit;
class CProjectile abstract
	: public CGameObject
{
protected:
	CProjectile();
	CProjectile(const CProjectile& _origin);
	virtual ~CProjectile();

public:
	virtual void	Projectile_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Projectile_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Projectile_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	void		Reset(CGameObject* pGameObject);
	_float4		Get_ArrowHeadPos();
	_float		Get_MaxDistance() { return m_fMaxDistance; }
public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();


public:
	enum ePROJECTILE_PHASE { eSTART, eLOOP, eSHOOT, eHIT, eSTICK, eEND };
	void		On_ShootProjectile();
	void			On_ChangePhase(ePROJECTILE_PHASE eNextPhase);

protected:
	_hashcode	m_hcCode = 0;

protected:
	CUnit* m_pOwnerUnit = nullptr;
	CHierarchyNode* m_pRightHandBone = nullptr;
	CHierarchyNode* m_pLeftHandBone = nullptr;
	CHierarchyNode* m_pCurStickBone = nullptr;

	_float4 m_vStartPosition = ZERO_VECTOR;
	_float4 m_vArrowHeadPos = ZERO_VECTOR;

	CGameObject* m_pHitUnit = nullptr;
	_float4x4 m_matHitOffset;

	
protected:
	_float	m_fMaxSpeed = 50.f;
	_float	m_fLoopTimeAcc = 0.f;
	_float	m_fMaxLoopTime = 3.f;
	_float	m_fMaxDistance = 50.f;

protected:
	ePROJECTILE_PHASE	m_eCurPhase = eSTART;

protected:
	HRESULT	SetUp_Projectile(wstring wstrModelFilePath);
	HRESULT	SetUp_Colliders(COL_GROUP_CLIENT eColType);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	void	Hit_Unit(CGameObject* pHitUnit);

private:
	_bool	m_bCloned = false;
	PxConvexMesh* m_pConvexMesh = nullptr;
	PxRigidDynamic* m_pActor = nullptr;
};

END