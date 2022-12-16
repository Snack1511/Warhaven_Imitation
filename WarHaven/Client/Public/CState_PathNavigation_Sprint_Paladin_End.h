#pragma once
#include "CState_PathNavigation_Sprint_Paladin.h"

BEGIN(Client)
class CState_PathNavigation_Sprint_Paladin_End
	: public CState_PathNavigation_Sprint_Paladin
{
	DECLARE_STATE(CState_PathNavigation_Sprint_Paladin_End);

protected:
	CState_PathNavigation_Sprint_Paladin_End();
	virtual ~CState_PathNavigation_Sprint_Paladin_End();

public:
	static CState_PathNavigation_Sprint_Paladin_End* Create();

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