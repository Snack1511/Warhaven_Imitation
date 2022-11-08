#pragma once
#include "CState.h"

BEGIN(Client)
class CWalk_Player
	: public CState
{
	DECLARE_STATE(CWalk_Player);

private:
	CWalk_Player();
	virtual ~CWalk_Player();

public:
	static CWalk_Player* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END