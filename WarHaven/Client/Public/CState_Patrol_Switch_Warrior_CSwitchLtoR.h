#pragma once
#include "CState_Patrol_Switch_Warrior.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Switch_Warrior_CSwitchLtoR
	: public CState_Patrol_Switch_Warrior
{
	DECLARE_STATE(CState_Patrol_Switch_Warrior_CSwitchLtoR);

private:
	CState_Patrol_Switch_Warrior_CSwitchLtoR();
	virtual ~CState_Patrol_Switch_Warrior_CSwitchLtoR();

public:
	static CState_Patrol_Switch_Warrior_CSwitchLtoR* Create();

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