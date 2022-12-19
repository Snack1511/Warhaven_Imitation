#pragma once
#include "CUnit.h"

BEGIN(Client)

class CAnimWeapon;
class CProjectile;

class CUnit_Archer
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Archer);


private:
	CUnit_Archer();
	virtual ~CUnit_Archer();

public:
	static	CUnit_Archer* Create(const UNIT_MODEL_DATA& tUnitModelData);

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

public:
	void			Set_ColorController(_uint iMeshPartType);

public:
	void			Create_DefaultArrow();
	void			Create_PurpleArrow();
	void			Create_SnipeArrow(_hashcode _hsCArrowCode);
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


};

END