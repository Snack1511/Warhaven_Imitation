#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_End
	: public CState
{
	DECLARE_STATE(CSprint_End);

private:
	CSprint_End();
	virtual ~CSprint_End();

public:
	static CSprint_End* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


};

END