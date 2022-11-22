#pragma once
#include "CState.h"

BEGIN(Client)
class CBounce_Warrior_AI abstract
	: public CState
{

protected:
	CBounce_Warrior_AI();
	virtual ~CBounce_Warrior_AI();


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