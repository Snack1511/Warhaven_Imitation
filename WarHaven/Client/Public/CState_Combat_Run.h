#pragma once
#include "CState_Combat.h"

BEGIN(Client)
class CState_Combat_Run abstract
	: public CState_Combat
{

protected:
	CState_Combat_Run();
	virtual ~CState_Combat_Run();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	_uint m_iRunDelay = 0;

};

END