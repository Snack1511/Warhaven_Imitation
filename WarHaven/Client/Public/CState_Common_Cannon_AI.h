#pragma once
#include "CState_Common.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Common_Cannon_AI
	: public CState_Common
{
	DECLARE_STATE(CState_Common_Cannon_AI);

private:
	CState_Common_Cannon_AI();
	virtual ~CState_Common_Cannon_AI();


public:
	static CState_Common_Cannon_AI* Create();

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