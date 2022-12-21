#pragma once
#include "CState_Patrol_Walk_Archer.h"

BEGIN(Client)
class CState_Patrol_Walk_Archer_L
	: public CState_Patrol_Walk_Archer
{
	DECLARE_STATE(CState_Patrol_Walk_Archer_L);

protected:
	CState_Patrol_Walk_Archer_L();
	virtual ~CState_Patrol_Walk_Archer_L();

public:
	static CState_Patrol_Walk_Archer_L* Create();

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