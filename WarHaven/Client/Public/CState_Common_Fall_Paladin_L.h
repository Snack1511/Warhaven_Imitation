#pragma once
#include "CState_Common_Fall_Paladin.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CState_Common_Fall_Paladin_L final
	: public CState_Common_Fall_Paladin
{
	DECLARE_STATE(CState_Common_Fall_Paladin_L);

private:
	CState_Common_Fall_Paladin_L();
	virtual ~CState_Common_Fall_Paladin_L();

public:
	static CState_Common_Fall_Paladin_L* Create();

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