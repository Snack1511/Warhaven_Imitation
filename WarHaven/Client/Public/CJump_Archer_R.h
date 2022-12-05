#pragma once
#include "CJump_Archer.h"


BEGIN(Client)
class CJump_Archer_R
	: public CJump_Archer
{
	DECLARE_STATE(CJump_Archer_R);

private:
	CJump_Archer_R();
	virtual ~CJump_Archer_R();

public:
	static CJump_Archer_R* Create();

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