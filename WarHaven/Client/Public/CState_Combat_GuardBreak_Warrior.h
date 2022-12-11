#pragma once
#include "CState_Combat_SkillE.h"

BEGIN(Client)
class CColorController;

class CState_Combat_GuardBreak_Warrior
	: public CState_Combat_SkillE
{
	DECLARE_STATE(CState_Combat_GuardBreak_Warrior);

private:
	CState_Combat_GuardBreak_Warrior();
	virtual ~CState_Combat_GuardBreak_Warrior();

public:
	static CState_Combat_GuardBreak_Warrior* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos) override;
	
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

	
};

END