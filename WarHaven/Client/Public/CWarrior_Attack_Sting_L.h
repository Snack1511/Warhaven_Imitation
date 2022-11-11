#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_Sting_L
	: public CState
{
	DECLARE_STATE(CWarrior_Attack_Sting_L);

private:
	CWarrior_Attack_Sting_L();
	virtual ~CWarrior_Attack_Sting_L();

public:
	static CWarrior_Attack_Sting_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	
};

END