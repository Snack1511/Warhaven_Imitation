#pragma once
#include "CState_Combat_Run.h"

BEGIN(Client)
class CState_Combat_Run_Warrior abstract
	: public CState_Combat_Run
{

protected:
	CState_Combat_Run_Warrior();
	virtual ~CState_Combat_Run_Warrior();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

};

END