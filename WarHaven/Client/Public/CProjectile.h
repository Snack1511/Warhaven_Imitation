#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

//#define TESTLEVEL_AI_PROJECTILE

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)
class CUnit;
class CTrailEffect;
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

	void		Set_TargetUnit(CUnit* pTargetUnit) { m_pTargetUnit = pTargetUnit; }

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();


public:
	enum ePROJECTILE_PHASE { eSTART, eLOOP, eSHOOT, eHIT, eSTICK, eEND };
	void		On_ShootProjectile();
	void		On_ChangePhase(ePROJECTILE_PHASE eNextPhase);

public:
	void	Use_Collect(_bool bCollect) { m_bCollect = bCollect; }

protected:
	_hashcode	m_hcCode = 0;

protected:
	CUnit* m_pOwnerUnit = nullptr;
	CUnit* m_pTargetUnit = nullptr;
	CHierarchyNode* m_pRightHandBone = nullptr;
	CHierarchyNode* m_pLeftHandBone = nullptr;
	CHierarchyNode* m_pCurStickBone = nullptr;

	_float4 m_vStartPosition = ZERO_VECTOR;
	_float4 m_vArrowHeadPos = ZERO_VECTOR;

	CGameObject* m_pHitUnit = nullptr;
	_float4x4 m_matHitOffset;

	// 100
	_bool		m_bFrontPhysX = false;

protected:
	_float	m_fMaxSpeed = 50.f;
	_float	m_fLoopTimeAcc = 0.f;
	_float	m_fMaxLoopTime = 3.f;
	_float	m_fMaxDistance = 50.f;
	_float	m_fDamage = 0.f;

protected:
	string m_szMainBoneName = "0B_R_WP1";
	string m_szSubBoneName = "0B_L_WP1";

	_bool  m_bCollect = true;

protected:
	_bool  m_bUseOwnerUnitLook = false;

protected:
	ePROJECTILE_PHASE	m_eCurPhase = eSTART;

protected:
	CTrailEffect* m_pTrailEffect = nullptr;
	CTrailEffect* m_pTrailEffect2 = nullptr;

protected:
	void SetUp_TrailEffect(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount);
	HRESULT	SetUp_Projectile(wstring wstrModelFilePath);
	HRESULT	SetUp_Colliders(COL_GROUP_CLIENT eColType); 
	HRESULT	SetUp_Collider(COL_GROUP_CLIENT eColType, _float fRadian);


protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	void	Hit_Unit(CGameObject* pHitUnit, _float4 vHitPos);

private:
	_bool	m_bCloned = false;
	PxConvexMesh* m_pConvexMesh = nullptr;
	PxRigidDynamic* m_pActor = nullptr;

private:
	virtual void Set_ColliderType(eTEAM_TYPE eTeamType);
};

END