#pragma once
#include "CUnit.h"

BEGIN(Client)

class CAnimWeapon;
class CProjectile;
class CUI_Trail;

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
	void			Enable_Arrow(_bool bEnable);
	void			Enable_Trail(_bool bEnable);
	void			ReMap_Trail(_float4 vTargetPos);

public:
	void			Set_ColorController(_uint iMeshPartType);

	_float4x4& Get_CoreMat() { return m_CoreMat; }

public:
	void			Create_DefaultArrow();
	void			Create_SnipeArrow();
	void			Change_ArrowPhase(_uint iPhase);
	void			Shoot_Arrow();

public:
	void	Collect_Arrow(_hashcode _hcCode, CProjectile* pEffect);
	CProjectile* Get_CurArrow() { return m_pCurArrow; }

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
	map<_hashcode, list<CProjectile*>>	m_mapProjectilePool;
	CProjectile* m_pCurArrow = nullptr;

	_float4x4	m_CoreMat;
	CUI_Trail* m_pUI_Trail = nullptr;

};

END