#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_Paladin
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Paladin);

private:
	CUnit_Paladin();
	virtual ~CUnit_Paladin();

public:
	static	CUnit_Paladin* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	virtual void On_Die() override;

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(UNIT_TYPE eUnitType);
	virtual void	SetUp_ReserveState(UNIT_TYPE eUnitType);
	virtual void	On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc);

public:
	void Turn_RushEffect(_bool bOnOff);

protected:
	virtual void	Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos) override;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_LateTick();

private:
	list<CGameObject*> m_RushEffects;
};

END