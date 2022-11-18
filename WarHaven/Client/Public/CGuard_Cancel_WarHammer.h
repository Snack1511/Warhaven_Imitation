#pragma once
#include "CState.h"

BEGIN(Client)
class CGuard_Cancel_WarHammer
	: public CState
{
	DECLARE_STATE(CGuard_Cancel_WarHammer);

private:
	CGuard_Cancel_WarHammer();
	virtual ~CGuard_Cancel_WarHammer();

public:
	static CGuard_Cancel_WarHammer* Create();

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