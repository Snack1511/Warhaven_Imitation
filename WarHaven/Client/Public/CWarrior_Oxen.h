#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Oxen
	: public CState
{
	DECLARE_STATE(CWarrior_Oxen);

private:
	CWarrior_Oxen();
	virtual ~CWarrior_Oxen();

public:
	static CWarrior_Oxen* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	void Play_Skill(_uint iChangeIndex, _float fInterPolationTime, CUnit* pOwner, CAnimator* pAnimator);

private:
	_int	m_iCurFinishedFrame = 0; // Finished 가 3번 호출되면 황소베기가 취소된다.
	_int	m_iEndFinishedFrame = 0;

	_float  m_fCreateTime = 0.016f;
	_float  m_fCreateTimeAcc = 0.f;

	_int	m_iCancelAnimIndex = 0;

private:
	virtual STATE_TYPE		Update_Begin(CUnit* pOwner, CAnimator* pAnimator);
	virtual STATE_TYPE		Update_Loop(CUnit* pOwner, CAnimator* pAnimator);
	virtual STATE_TYPE		Update_End(CUnit* pOwner, CAnimator* pAnimator);

};

END