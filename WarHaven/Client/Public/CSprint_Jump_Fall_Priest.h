#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_Jump_Fall_Priest
	: public CState
{
	DECLARE_STATE(CSprint_Jump_Fall_Priest);

private:
	CSprint_Jump_Fall_Priest();
	virtual ~CSprint_Jump_Fall_Priest();

public:
	static CSprint_Jump_Fall_Priest* Create();

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