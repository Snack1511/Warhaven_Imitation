#pragma once
#include "CState_Combat_SkillE.h"

BEGIN(Client)
class CColorController;

class CState_Combat_SkillE_Priest_WindAttack
	: public CState_Combat_SkillE
{
	DECLARE_STATE(CState_Combat_SkillE_Priest_WindAttack);

private:
	CState_Combat_SkillE_Priest_WindAttack();
	virtual ~CState_Combat_SkillE_Priest_WindAttack();

public:
	static CState_Combat_SkillE_Priest_WindAttack* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	_uint m_iAttackStartAnimIndex = 0;
	_uint m_iAttackEndAnimIndex = 0;

};

END