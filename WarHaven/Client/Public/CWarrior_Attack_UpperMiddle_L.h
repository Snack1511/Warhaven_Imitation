#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_UpperMiddle_L
	: public CState
{
	DECLARE_STATE(CWarrior_Attack_UpperMiddle_L);

private:
	CWarrior_Attack_UpperMiddle_L();
	virtual ~CWarrior_Attack_UpperMiddle_L();

public:
	static CWarrior_Attack_UpperMiddle_L* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_int	m_iFinishedFrame = 0; // Finished �� 3�� ȣ��Ǹ� Ȳ�Һ��Ⱑ ��ҵȴ�.
	_float  m_fCreateTime = 0.016f;
	_float  m_fCreateTimeAcc = 0.f;
	
};

END