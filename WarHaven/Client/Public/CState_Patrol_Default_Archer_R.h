#pragma once
#include "CState_Patrol_Default_Archer.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Default_Archer_R
	: public CState_Patrol_Default_Archer
{
	DECLARE_STATE(CState_Patrol_Default_Archer_R);

private:
	CState_Patrol_Default_Archer_R();
	virtual ~CState_Patrol_Default_Archer_R();

public:
	static CState_Patrol_Default_Archer_R* Create();

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