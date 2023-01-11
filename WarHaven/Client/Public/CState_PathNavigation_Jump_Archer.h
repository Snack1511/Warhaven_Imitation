#pragma once
#include "CState_PathNavigation_Jump.h"

BEGIN(Client)
class CState_PathNavigation_Jump_Archer abstract
	: public CState_PathNavigation_Jump
{

protected:
	CState_PathNavigation_Jump_Archer();
	virtual ~CState_PathNavigation_Jump_Archer();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


//protected:
//	void	Target_Near_ChangeState(_uint );

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;




};

END