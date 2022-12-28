#pragma once
#include "CState.h"


BEGIN(Client)
class CJump_Priest
	: public CState
{
	DECLARE_STATE(CJump_Priest);

private:
	CJump_Priest();
	virtual ~CJump_Priest();

public:
	static CJump_Priest* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_uint iPlaceJumpAnimIndex = 0;

};

END