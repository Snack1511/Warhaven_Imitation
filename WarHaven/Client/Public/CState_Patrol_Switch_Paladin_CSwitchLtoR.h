#pragma once
#include "CState_Patrol_Switch_Paladin.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Switch_Paladin_CSwitchLtoR
	: public CState_Patrol_Switch_Paladin
{
	DECLARE_STATE(CState_Patrol_Switch_Paladin_CSwitchLtoR);

private:
	CState_Patrol_Switch_Paladin_CSwitchLtoR();
	virtual ~CState_Patrol_Switch_Paladin_CSwitchLtoR();

public:
	static CState_Patrol_Switch_Paladin_CSwitchLtoR* Create();

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