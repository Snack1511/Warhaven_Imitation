#pragma once
#include "CWarrior_Attack_HorizontalMiddle.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_HorizontalMiddle_L
	: public CWarrior_Attack_HorizontalMiddle
{
	DECLARE_STATE(CWarrior_Attack_HorizontalMiddle_L);

private:
	CWarrior_Attack_HorizontalMiddle_L();
	virtual ~CWarrior_Attack_HorizontalMiddle_L();

public:
	static CWarrior_Attack_HorizontalMiddle_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);
	
};

END