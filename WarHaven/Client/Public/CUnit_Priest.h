#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_Priest
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Priest);

private:
	CUnit_Priest();
	virtual ~CUnit_Priest();

public:
	static	CUnit_Priest* Create(const UNIT_MODEL_DATA& tUnitModelData);




public:
	virtual void On_Die() override;

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(UNIT_TYPE eUnitType);
	virtual void	SetUp_ReserveState(UNIT_TYPE eUnitType);
	virtual void	On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc);

public:
	void SetUp_CureEffect();
	void Turn_CatchEffet(_bool bOnOff);
	void TurnOn_CureEffect(_bool bOnOff);
	void TurnOff_AllEffect();

protected:
	virtual void	Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos) override;

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
	STATE_TYPE m_eCureBeginType = STATE_END;
	STATE_TYPE m_eCureLoopType = STATE_END;

private:
	list<CGameObject*> m_CatchEffect;
	list<CGameObject*> m_CatchMeshEffect;
	CGameObject* m_pCureEffect = nullptr;

};

END