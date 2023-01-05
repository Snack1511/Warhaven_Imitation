#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalMiddle_Paladin.h"

#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"

#include "CEffects_Factory.h"

CState_Combat_Attack_HorizontalMiddle_Paladin::CState_Combat_Attack_HorizontalMiddle_Paladin()
{
}

CState_Combat_Attack_HorizontalMiddle_Paladin::~CState_Combat_Attack_HorizontalMiddle_Paladin()
{
}

HRESULT CState_Combat_Attack_HorizontalMiddle_Paladin::Initialize()
{
	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 1.5f;

	m_iStateChangeKeyFrame = 76;

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;

	m_iStopIndex = 50;
	

	Add_KeyFrame(39, 1);
	Add_KeyFrame(m_iStopIndex, 2);
	Add_KeyFrame(39, 999);


	return __super::Initialize();
}

void CState_Combat_Attack_HorizontalMiddle_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE eRandState = Enter_RandomState(pOwner, pAnimator);

	if (eRandState != STATE_END)
		return eRandState;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalMiddle_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_Attack_HorizontalMiddle_Paladin::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


}

