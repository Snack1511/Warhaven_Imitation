
#pragma once
#include "CState_PathNavigation_Fall.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CState_PathNavigation_Fall_Qanda
	: public CState_PathNavigation_Fall
{
	DECLARE_STATE(CState_PathNavigation_Fall_Qanda);

private:
	CState_PathNavigation_Fall_Qanda();
	virtual ~CState_PathNavigation_Fall_Qanda();

public:
	static CState_PathNavigation_Fall_Qanda* Create();

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
