#pragma once
#include "CState_Combat_SkillR.h"


BEGIN(Client)
class CState_Combat_SkillR_AirDash_Priest
	: public CState_Combat_SkillR
{
	DECLARE_STATE(CState_Combat_SkillR_AirDash_Priest);

private:
	CState_Combat_SkillR_AirDash_Priest();
	virtual ~CState_Combat_SkillR_AirDash_Priest();

public:
	static CState_Combat_SkillR_AirDash_Priest* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	_uint iPlaceJumpAnimIndex = 0;
	_bool m_bLoop = false;

	_float4 m_vDir = ZERO_VECTOR;
};

END