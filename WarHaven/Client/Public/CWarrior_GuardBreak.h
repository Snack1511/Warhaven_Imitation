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
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

	
};

END