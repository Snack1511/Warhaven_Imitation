#pragma once
#include "CState.h"

BEGIN(Client)
class CGuard_Dash_Player
	: public CState
{
	DECLARE_STATE(CGuard_Dash_Player);

private:
	CGuard_Dash_Player();
	virtual ~CGuard_Dash_Player();

public:
	static CGuard_Dash_Player* Create();

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