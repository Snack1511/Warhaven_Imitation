#pragma once
#include "CRun_AI_TG_Warrior.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_AI_TG_Warrior_L
	: public CRun_AI_TG_Warrior
{
	DECLARE_STATE(CRun_AI_TG_Warrior_L);

private:
	CRun_AI_TG_Warrior_L();
	virtual ~CRun_AI_TG_Warrior_L();

public:
	static CRun_AI_TG_Warrior_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


};

END