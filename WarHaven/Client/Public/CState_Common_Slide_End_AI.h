#pragma once
#include "CState_Common.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CState_Common_Slide_End_AI
	: public CState_Common
{
	DECLARE_STATE(CState_Common_Slide_End_AI);

private:
	CState_Common_Slide_End_AI();
	virtual ~CState_Common_Slide_End_AI();

public:
	static CState_Common_Slide_End_AI* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

//private:
//	_bool	Change_Animation_Run(_uint iBeginAttackAnim, _uint iAttackAnim, _uint iAttackAnim, CAnimator* pAnimator);

private:
	_bool	m_bPlayRun = false;

};

END