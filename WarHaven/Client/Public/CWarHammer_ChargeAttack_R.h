#pragma once
#include "CWarHammer_Attack_HorizontalMiddle.h"

BEGIN(Client)
class CColorController;

class CWarHammer_ChargeAttack_R
	: public CWarHammer_Attack_HorizontalMiddle
{
	DECLARE_STATE(CWarHammer_ChargeAttack_R);

private:
	CWarHammer_ChargeAttack_R();
	virtual ~CWarHammer_ChargeAttack_R();

public:
	static CWarHammer_ChargeAttack_R* Create();

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