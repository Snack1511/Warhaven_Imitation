#pragma once
#include "CState.h"

BEGIN(Client)
class CJump_Archer abstract
	: public CState
{

protected:
	CJump_Archer();
	virtual ~CJump_Archer();


public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	_int	iPlaceJumpAnimIndex = 0; // 제자리 점프 AnimIndex;
	

private:
	_float	fOxenJumpPower = 0.f;

};

END