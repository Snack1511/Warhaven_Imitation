#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Oxen_Begin
	: public CState
{
	DECLARE_STATE(CWarrior_Oxen_Begin);

private:
	CWarrior_Oxen_Begin();
	virtual ~CWarrior_Oxen_Begin();

public:
	static CWarrior_Oxen_Begin* Create();

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

};

END