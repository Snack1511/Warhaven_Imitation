#pragma once
#include "CState_PathNavigation_Fall.h"

BEGIN(Client)
class CState_PathNavigation_Fall_Fiona abstract
	: public CState_PathNavigation_Fall
{

protected:
	CState_PathNavigation_Fall_Fiona();
	virtual ~CState_PathNavigation_Fall_Fiona();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence) {};
};

END