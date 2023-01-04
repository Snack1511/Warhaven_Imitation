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
	void			Create_Meteor();
	void			Change_CrowPhase(_uint iPhase);

public:
	void			Turn_TransformParticle(_bool bOnoff);

public:
	void	Collect_QandaProjectile(_hashcode _hcCode, CProjectile* pEffect);

public:
	// CGameObject을(를) 통해 상속됨
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
	CGameObject* m_pFeathers = nullptr;

	map<_hashcode, list<CProjectile*>>	m_mapProjectilePool;
	list<CProjectile*> m_pMeteor = { nullptr };

	_float4x4	m_CoreMat;
	CUI_Trail* m_pUI_Trail = nullptr;

};

END