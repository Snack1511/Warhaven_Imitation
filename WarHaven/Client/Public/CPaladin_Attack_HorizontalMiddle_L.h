#pragma once
#include "CPaladin_Attack_HorizontalMiddle.h"

BEGIN(Client)
class CColorController;

class CPaladin_Attack_HorizontalMiddle_L
	: public CPaladin_Attack_HorizontalMiddle
{
	DECLARE_STATE(CPaladin_Attack_HorizontalMiddle_L);

private:
	CPaladin_Attack_HorizontalMiddle_L();
	virtual ~CPaladin_Attack_HorizontalMiddle_L();

public:
	static CPaladin_Attack_HorizontalMiddle_L* Create();

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