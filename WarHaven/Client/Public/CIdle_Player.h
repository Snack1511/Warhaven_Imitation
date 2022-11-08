#pragma once
#include "CState.h"

BEGIN(Client)
class CIdle_Player
	: public CState
{
	DECLARE_STATE(CIdle_Player);

private:
	CIdle_Player();
	virtual ~CIdle_Player();

public:
	static CIdle_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	void	Test_Animation_Loop(CUnit* pOwner, CAnimator* pAnimator);

};

END