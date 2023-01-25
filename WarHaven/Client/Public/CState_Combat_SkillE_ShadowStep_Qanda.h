#pragma once
#include "CState_Combat_SkillE.h"

BEGIN(Engine)
class CAnimator;
class CGameObject;
END

BEGIN(Client)
class CState_Combat_SkillE_ShadowStep_Qanda
	: public CState_Combat_SkillE
{
	DECLARE_STATE(CState_Combat_SkillE_ShadowStep_Qanda);

private:
	CState_Combat_SkillE_ShadowStep_Qanda();
	virtual ~CState_Combat_SkillE_ShadowStep_Qanda();

public:
	static CState_Combat_SkillE_ShadowStep_Qanda* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	void TurnOff_DodgeEffect(CUnit* pOwner);

private:
	_bool m_bJump = false;
	_uint m_iStack = 0;
	_float m_fCurGuageMinusTime = 0.f;
	_float m_fMaxGuageMinusTime = 0.1f;
	_float m_fWalkSpeed = 0.f;

	list<CGameObject*>	m_EffectsList;

	_float m_fSndTime = 0.f;
	_float m_iSndIdx = 0;
};

END