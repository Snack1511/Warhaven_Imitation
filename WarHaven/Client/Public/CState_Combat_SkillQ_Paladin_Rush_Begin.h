#pragma once
#include "CState_Combat_SkillQ_Paladin.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CState_Combat_SkillQ_Paladin_Rush_Begin
	: public CState_Combat_SkillQ_Paladin
{
	DECLARE_STATE(CState_Combat_SkillQ_Paladin_Rush_Begin);

private:
	CState_Combat_SkillQ_Paladin_Rush_Begin();
	virtual ~CState_Combat_SkillQ_Paladin_Rush_Begin();

public:
	static CState_Combat_SkillQ_Paladin_Rush_Begin* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence) override;
};

END