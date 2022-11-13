#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_UpperMiddle_R
	: public CState
{
	DECLARE_STATE(CWarrior_Attack_UpperMiddle_R);

private:
	CWarrior_Attack_UpperMiddle_R();
	virtual ~CWarrior_Attack_UpperMiddle_R();

public:
	static CWarrior_Attack_UpperMiddle_R* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	_bool	m_bCanMove = true;

	_int	m_iFinishedFrame = 0; // Finished 가 3번 호출되면 황소베기가 취소된다.
	_float  m_fCreateTime = 0.016f;
	_float  m_fCreateTimeAcc = 0.f;
	
};

END