#pragma once
#include "CJump_WarHammer.h"


BEGIN(Client)
class CJump_WarHammer_L
	: public CJump_WarHammer
{
	DECLARE_STATE(CJump_WarHammer_L);

private:
	CJump_WarHammer_L();
	virtual ~CJump_WarHammer_L();

public:
	static CJump_WarHammer_L* Create();

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