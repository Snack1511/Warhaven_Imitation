#pragma once
#include "CState.h"

BEGIN(Client)
class CSprintAttack_Priest_Begin
	: public CState
{
	DECLARE_STATE(CSprintAttack_Priest_Begin);

private:
	CSprintAttack_Priest_Begin();
	virtual ~CSprintAttack_Priest_Begin();

public:
	static CSprintAttack_Priest_Begin* Create();

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