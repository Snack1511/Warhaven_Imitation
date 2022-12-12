#pragma once
#include "CState_Common.h"

BEGIN(Client)
class CColorController;

class CState_Common_ChangeHero_AI
	: public CState_Common
{
	DECLARE_STATE(CState_Common_ChangeHero_AI);

private:
	CState_Common_ChangeHero_AI();
	virtual ~CState_Common_ChangeHero_AI();

public:
	static CState_Common_ChangeHero_AI* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	CLASS_TYPE	m_eChangeClassType = FIONA;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

	STATE_TYPE Set_HeroType(CUnit* pOwner, CLASS_TYPE eClass);
};

END