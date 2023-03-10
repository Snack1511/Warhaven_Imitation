#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CChangeHero_Player
	: public CState
{
	DECLARE_STATE(CChangeHero_Player);

private:
	CChangeHero_Player();
	virtual ~CChangeHero_Player();

public:
	static CChangeHero_Player* Create();

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