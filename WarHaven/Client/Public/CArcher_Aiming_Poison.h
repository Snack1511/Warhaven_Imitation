#pragma once
#include "CState_Blendable.h"

BEGIN(Client)
class CColorController;

class CArcher_Aiming_Poison
	: public CState_Blendable
{
	DECLARE_STATE(CArcher_Aiming_Poison);

private:
	CArcher_Aiming_Poison();
	virtual ~CArcher_Aiming_Poison();

public:
	static CArcher_Aiming_Poison* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

};

END