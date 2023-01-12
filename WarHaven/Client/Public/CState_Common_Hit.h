#pragma once
#include "CState_Hit.h"
#include "CState_Common.h"

BEGIN(Client)
class CState_Common_Hit abstract
	: public CState_Common

{

protected:
	CState_Common_Hit();
	virtual ~CState_Common_Hit();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence) {};

protected:
	_bool m_bSting = false;

};

END