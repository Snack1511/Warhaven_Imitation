#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
class CGameObject;
END

BEGIN(Client)
class CQanda_ShadowStep
	: public CState
{
	DECLARE_STATE(CQanda_ShadowStep);

private:
	CQanda_ShadowStep();
	virtual ~CQanda_ShadowStep();

public:
	static CQanda_ShadowStep* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_uint m_iCurDirection = 0;
	_float m_fCurGuageMinusTime = 0.f;
	_float m_fMaxGuageMinusTime = 0.1f;

	list<CGameObject*>	m_EffectsList;
	
};

END