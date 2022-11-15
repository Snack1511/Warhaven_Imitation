#pragma once
#include "CState_Blendable.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_Sting_R
	: public CState_Blendable
{
	DECLARE_STATE(CWarrior_Attack_Sting_R);

private:
	CWarrior_Attack_Sting_R();
	virtual ~CWarrior_Attack_Sting_R();

public:
	static CWarrior_Attack_Sting_R* Create();

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