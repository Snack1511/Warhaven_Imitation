#pragma once
#include "CState_Combat_Default_Fiona.h"

BEGIN(Client)
class CState_Combat_Default_Fiona_L
	: public CState_Combat_Default_Fiona
{
	DECLARE_STATE(CState_Combat_Default_Fiona_L);

private:
	CState_Combat_Default_Fiona_L();
	virtual ~CState_Combat_Default_Fiona_L();

public:
	static CState_Combat_Default_Fiona_L* Create();

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