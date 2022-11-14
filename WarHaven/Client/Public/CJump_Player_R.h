#pragma once
#include "CJump_Player.h"


BEGIN(Client)
class CJump_Player_R
	: public CJump_Player
{
	DECLARE_STATE(CJump_Player_R);

private:
	CJump_Player_R();
	virtual ~CJump_Player_R();

public:
	static CJump_Player_R* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


};

END