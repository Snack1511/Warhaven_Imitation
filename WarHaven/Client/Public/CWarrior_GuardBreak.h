#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_GuardBreak
	: public CState
{
	DECLARE_STATE(CWarrior_GuardBreak);

private:
	CWarrior_GuardBreak();
	virtual ~CWarrior_GuardBreak();

public:
	static CWarrior_GuardBreak* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float  m_fCreateTime = 0.016f;
	_float  m_fCreateTimeAcc = 0.f;
	
};

END