#pragma once
#include "CState_Combat_SkillR.h"

BEGIN(Client)
class CColorController;

class CState_Combat_SkillR_Archer_Swing
	: public CState_Combat_SkillR
{
	DECLARE_STATE(CState_Combat_SkillR_Archer_Swing);

private:
	CState_Combat_SkillR_Archer_Swing();
	virtual ~CState_Combat_SkillR_Archer_Swing();

public:
	static CState_Combat_SkillR_Archer_Swing* Create();

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