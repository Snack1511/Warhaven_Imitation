#pragma once
#include "CState_Patrol_Walk.h"

BEGIN(Client)
class CState_Patrol_Walk_Fiona abstract
	: public CState_Patrol_Walk
{

protected:
	CState_Patrol_Walk_Fiona();
	virtual ~CState_Patrol_Walk_Fiona();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

};

END