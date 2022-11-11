#pragma once
#include "CState.h"

BEGIN(Client)
class CRun_Player abstract
	: public CState
{

protected:
	CRun_Player();
	virtual ~CRun_Player();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	_int	m_VecDirectionAnimIndex[STATE_DIRECTION_END];
	_int	m_VecDirectionBeginAnimIndex[STATE_DIRECTION_END];

};

END