#pragma once
#include "CState_PathNavigation.h"

BEGIN(Client)
class CState_PathNavigation_Sprint abstract
	: public CState_PathNavigation
{

protected:
	CState_PathNavigation_Sprint();
	virtual ~CState_PathNavigation_Sprint();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

protected:
	_uint m_iEscapeFrame = 60;
};

END