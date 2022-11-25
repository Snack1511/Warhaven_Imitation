#pragma once
#include "CAI_TG_Warrior_Attack_HorizontalMiddle.h"

BEGIN(Client)
class CColorController;

class CAI_TG_Warrior_Attack_HorizontalMiddle_L
	: public CAI_TG_Warrior_Attack_HorizontalMiddle
{
	DECLARE_STATE(CAI_TG_Warrior_Attack_HorizontalMiddle_L);

private:
	CAI_TG_Warrior_Attack_HorizontalMiddle_L();
	virtual ~CAI_TG_Warrior_Attack_HorizontalMiddle_L();

public:
	static CAI_TG_Warrior_Attack_HorizontalMiddle_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);
	
};

END