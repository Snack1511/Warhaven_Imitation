#pragma once
#include "CState.h"

BEGIN(Client)
class CSprintAttack_Player_Begin
	: public CState
{
	DECLARE_STATE(CSprintAttack_Player_Begin);

private:
	CSprintAttack_Player_Begin();
	virtual ~CSprintAttack_Player_Begin();

public:
	static CSprintAttack_Player_Begin* Create();

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