#pragma once
#include "CUnit.h"

BEGIN(Client)

class CAnimWeapon;
class CProjectile;
class CUI_Trail;
class CAnimWeapon_Crow;

class CUnit_Qanda
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Qanda);


private:
	CUnit_Qanda();
	virtual ~CUnit_Qanda();

public:
	static	CUnit_Qanda* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	virtual void On_Die() override;

public:
	void TurnOn_Trail(_bool bOn);

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(UNIT_TYPE eUnitType);
	virtual void	SetUp_ReserveState(UNIT_TYPE eUnitType);
	virtual void	On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc);

protected:
	virtual void	Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos) override;

public:
	void Set_CrowAnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed);
	void On_ChangePhase(_uint eCurPhase);
	void Shoot_AnimCrow();
	CAnimWeapon_Crow* Get_Crow() { return m_pAnimCrow; }

public:
	void			Enable_Trail(_bool bEnable);
	void			ReMap_Trail(_float4 vTargetPos);

	_float4x4& Get_CoreMat() { return m_CoreMat; }

public:
	CGameObject*	Create_Meteor();
	//void			Change_CrowPhase(_uint iPhase);

public:
	void			Turn_TransformParticle(_bool bOnOff);
	void			Turn_ChargeEffect(_bool bOnOff);
	void			Turn_FeatherEffect(_bool bOnOff);
	void			Turn_SteamEffect(_bool bOnOff);
	void			TurnOff_AllEffect();

public:
	void	Collect_QandaProjectile(_hashcode _hcCode, CProjectile* pEffect);

protected:
	void SetUp_Trail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_Trail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_LowerTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_LowerTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	CAnimWeapon_Crow* m_pAnimCrow = nullptr;
	list<CGameObject*> m_TransformParticles;
	list<CGameObject*> m_ChargeEffect;
	list<CGameObject*> m_SteamEffect;
	CGameObject* m_pFeathers = nullptr;
	CGameObject* m_pChargeParticle = nullptr;

	map<_hashcode, list<CProjectile*>>	m_mapProjectilePool;
	CProjectile* m_pMeteor =  nullptr ;

	_float4x4	m_CoreMat;
	CUI_Trail* m_pUI_Trail = nullptr;

	_float4 m_vTrailShader;
	CTrailEffect* m_pTrail_R = nullptr;
	CTrailEffect* m_pTrail_R2 = nullptr;
	CTrailEffect* m_pTrail_L = nullptr;
	CTrailEffect* m_pTrail_L2 = nullptr;

	CTrailEffect* m_pLowerTrail_R = nullptr;
	CTrailEffect* m_pLowerTrail_R2 = nullptr;
	CTrailEffect* m_pLowerTrail_L = nullptr;
	CTrailEffect* m_pLowerTrail_L2 = nullptr;



};

END