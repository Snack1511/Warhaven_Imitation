#pragma once
#include "CState_Combat_Run.h"

BEGIN(Client)
class CState_Combat_Default_Priest
	: public CState_Combat_Run
{
	DECLARE_STATE(CState_Combat_Default_Priest);

protected:
	CState_Combat_Default_Priest();
	virtual ~CState_Combat_Default_Priest();

public:
	static CState_Combat_Default_Priest* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	STATE_TYPE Random_State(CUnit* pOwner, CAnimator* pAnimator);

};

END