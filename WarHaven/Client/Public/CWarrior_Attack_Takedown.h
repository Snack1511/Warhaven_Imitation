#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_Takedown
	: public CState
{
	DECLARE_STATE(CWarrior_Attack_Takedown);

private:
	CWarrior_Attack_Takedown();
	virtual ~CWarrior_Attack_Takedown();

public:
	static CWarrior_Attack_Takedown* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	
};

END