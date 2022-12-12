#pragma once
#include "CState_Patrol_Switch_Engineer.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Switch_Engineer_CSwitchRtoL
	: public CState_Patrol_Switch_Engineer
{
	DECLARE_STATE(CState_Patrol_Switch_Engineer_CSwitchRtoL);

private:
	CState_Patrol_Switch_Engineer_CSwitchRtoL();
	virtual ~CState_Patrol_Switch_Engineer_CSwitchRtoL();

public:
	static CState_Patrol_Switch_Engineer_CSwitchRtoL* Create();

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