#pragma once
#include "CState_Combat_Run_Warrior.h"

BEGIN(Client)
class CState_Combat_Run_Warrior_R
	: public CState_Combat_Run_Warrior
{
	DECLARE_STATE(CState_Combat_Run_Warrior_R);

protected:
	CState_Combat_Run_Warrior_R();
	virtual ~CState_Combat_Run_Warrior_R();

public:
	static CState_Combat_Run_Warrior_R* Create();

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