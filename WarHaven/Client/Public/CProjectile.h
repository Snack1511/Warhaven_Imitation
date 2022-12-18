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
	virtual ~CProjectile();

public:
	virtual void	Projectile_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Projectile_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Projectile_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	void		Reset(CGameObject* pGameObject);


public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();


public:
	enum ePROJECTILE_PHASE { eSTART, eLOOP, eSHOOT, eHIT, eEND };
	void		On_ShootProjectile();
	void			On_ChangePhase(ePROJECTILE_PHASE eNextPhase);

protected:
	CUnit* m_pOwnerUnit = nullptr;
	CHierarchyNode* m_pRightHandBone = nullptr;
	CHierarchyNode* m_pLeftHandBone = nullptr;
	CHierarchyNode* m_pCurStickBone = nullptr;

	_float4 m_vHitOffsetPos = ZERO_VECTOR;
	_float4 m_vShootLook = ZERO_VECTOR;

protected:
	_float	m_fMaxSpeed = 50.f;
	_float	m_fLoopTimeAcc = 0.f;
	_float	m_fMaxLoopTime = 3.f;

protected:
	ePROJECTILE_PHASE	m_eCurPhase = eSTART;

protected:
	HRESULT	SetUp_Projectile(wstring wstrModelFilePath);
	HRESULT	SetUp_Colliders(COL_GROUP_CLIENT eColType);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
};

END