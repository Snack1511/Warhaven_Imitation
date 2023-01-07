#pragma once
#include "CState_PathNavigation_Jump.h"


BEGIN(Client)
class CState_PathNavigation_Jump_Priest
	: public CState_PathNavigation_Jump
{
	DECLARE_STATE(CState_PathNavigation_Jump_Priest);

private:
	CState_PathNavigation_Jump_Priest();
	virtual ~CState_PathNavigation_Jump_Priest();

public:
	static CState_PathNavigation_Jump_Priest* Create();

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