#pragma once
#include "CState_Common_Bounce.h"

BEGIN(Client)
class CState_Common_Bounce_Warrior abstract
	: public CState_Common_Bounce
{

protected:
	CState_Common_Bounce_Warrior();
	virtual ~CState_Common_Bounce_Warrior();


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