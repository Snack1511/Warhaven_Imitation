#pragma once
#include "CState_Combat_SkillQ.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CState_Combat_SkillQ_Begin_Sniping_Qanda
	: public CState_Combat_SkillQ
{
	DECLARE_STATE(CState_Combat_SkillQ_Begin_Sniping_Qanda);

private:
	CState_Combat_SkillQ_Begin_Sniping_Qanda();
	virtual ~CState_Combat_SkillQ_Begin_Sniping_Qanda();

public:
	static CState_Combat_SkillQ_Begin_Sniping_Qanda* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence) override;

private:


};

END