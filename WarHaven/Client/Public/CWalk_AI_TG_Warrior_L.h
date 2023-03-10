#pragma once
#include "CWalk_AI_TG_Warrior.h"

BEGIN(Client)
class CWalk_AI_TG_Warrior_L
	: public CWalk_AI_TG_Warrior
{
	DECLARE_STATE(CWalk_AI_TG_Warrior_L);

protected:
	CWalk_AI_TG_Warrior_L();
	virtual ~CWalk_AI_TG_Warrior_L();

public:
	static CWalk_AI_TG_Warrior_L* Create();

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