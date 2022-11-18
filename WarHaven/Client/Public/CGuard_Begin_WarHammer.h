#pragma once
#include "CState.h"

BEGIN(Client)
class CGuard_Begin_WarHammer
	: public CState
{
	DECLARE_STATE(CGuard_Begin_WarHammer);

private:
	CGuard_Begin_WarHammer();
	virtual ~CGuard_Begin_WarHammer();

public:
	static CGuard_Begin_WarHammer* Create();

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