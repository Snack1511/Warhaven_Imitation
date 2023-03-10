#pragma once
#include "CState_Combat_Attack_Paladin.h"

BEGIN(Client)
class CColorController;

class CState_Combat_Attack_VerticalCut_Paladin
	: public CState_Combat_Attack_Paladin
{
	DECLARE_STATE(CState_Combat_Attack_VerticalCut_Paladin);

private:
	CState_Combat_Attack_VerticalCut_Paladin();
	virtual ~CState_Combat_Attack_VerticalCut_Paladin();

public:
	static CState_Combat_Attack_VerticalCut_Paladin* Create();

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