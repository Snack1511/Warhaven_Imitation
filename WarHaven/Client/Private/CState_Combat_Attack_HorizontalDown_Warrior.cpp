#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalDown_Warrior.h"

#include "CUtility_Transform.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Attack_HorizontalDown_Warrior::CState_Combat_Attack_HorizontalDown_Warrior()
{
}

CState_Combat_Attack_HorizontalDown_Warrior::~CState_Combat_Attack_HorizontalDown_Warrior()
{
}

HRESULT CState_Combat_Attack_HorizontalDown_Warrior::Initialize()
{
	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 75;

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 10.f;

	Add_KeyFrame(40, 1);
	Add_KeyFrame(50, 2);
	Add_KeyFrame(45, 999);

	return S_OK;
}

void CState_Combat_Attack_HorizontalDown_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(m_eBounceState);

	pOwner->On_Attack(this);

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalDown_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_HorizontalDown_Warrior::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

