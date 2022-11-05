#pragma once
#include "CState.h"

BEGIN(Client)
class CJump_Enemy
	: public CState
{
	DECLARE_STATE(CJump_Enemy);
private:
	CJump_Enemy();
	virtual ~CJump_Enemy();

public:
	static CJump_Enemy* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END