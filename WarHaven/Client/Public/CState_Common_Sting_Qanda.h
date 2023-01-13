#pragma once
#include "CState_Common_Hit.h"

BEGIN(Engine)
END


BEGIN(Client)
class CState_Common_Sting_Qanda
	: public CState_Common_Hit
{
	DECLARE_STATE(CState_Common_Sting_Qanda);

private:
	CState_Common_Sting_Qanda();
	virtual ~CState_Common_Sting_Qanda();

public:
	static CState_Common_Sting_Qanda* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool	bTest = false; 

};

END