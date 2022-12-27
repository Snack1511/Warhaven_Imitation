#pragma once
#include "CState_Hit.h"

BEGIN(Client)
class CColorController;

class CHit_Qanda
	: public CState_Hit
{
	DECLARE_STATE(CHit_Qanda);

private:
	CHit_Qanda();
	virtual ~CHit_Qanda();

public:
	static CHit_Qanda* Create();

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