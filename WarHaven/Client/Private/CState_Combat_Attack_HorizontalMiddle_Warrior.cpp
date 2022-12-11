#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalMiddle_Warrior.h"

#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CState_Combat_Attack_HorizontalMiddle_Warrior::CState_Combat_Attack_HorizontalMiddle_Warrior()
{
}

CState_Combat_Attack_HorizontalMiddle_Warrior::~CState_Combat_Attack_HorizontalMiddle_Warrior()
{
}

HRESULT CState_Combat_Attack_HorizontalMiddle_Warrior::Initialize()
{
	m_tHitInfo.fKnockBackPower = 2.f;
	m_tHitInfo.fJumpPower = 0.f;

	m_fInterPolationTime = 0.1f;

	Add_KeyFrame(30, 1);
	Add_KeyFrame(50, 2);
	Add_KeyFrame(40, 999);

	return S_OK;
}

void CState_Combat_Attack_HorizontalMiddle_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(m_eBounceState);

	pOwner->On_Attack(this);

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalMiddle_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	return STATE_END;
}

void CState_Combat_Attack_HorizontalMiddle_Warrior::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

