#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CIdle_AI_TG_Warrior_R
	: public CState
{
	DECLARE_STATE(CIdle_AI_TG_Warrior_R);

private:
	CIdle_AI_TG_Warrior_R();
	virtual ~CIdle_AI_TG_Warrior_R();

public:
	static CIdle_AI_TG_Warrior_R* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float	fwaitCurTime = 0.f;
	_float	fwaitCoolTime = 0.6f;

};

END