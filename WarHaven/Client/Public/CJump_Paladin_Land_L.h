#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END
 
BEGIN(Client)
class CJump_Paladin_Land_L
	: public CState
{
	DECLARE_STATE(CJump_Paladin_Land_L);

private:
	CJump_Paladin_Land_L();
	virtual ~CJump_Paladin_Land_L();

public:
	static CJump_Paladin_Land_L* Create();

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