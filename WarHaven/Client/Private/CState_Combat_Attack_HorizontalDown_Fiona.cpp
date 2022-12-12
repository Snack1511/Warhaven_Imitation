#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalDown_Fiona.h"

#include "CUtility_Transform.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Attack_HorizontalDown_Fiona::CState_Combat_Attack_HorizontalDown_Fiona()
{
}

CState_Combat_Attack_HorizontalDown_Fiona::~CState_Combat_Attack_HorizontalDown_Fiona()
{
}

HRESULT CState_Combat_Attack_HorizontalDown_Fiona::Initialize()
{
	m_tHitInfo.fKnockBackPower = 2.f;
	m_tHitInfo.fJumpPower = 0.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 75;

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 10.f;


	Add_KeyFrame(40, 1);
	Add_KeyFrame(50, 2);
	Add_KeyFrame(45, 999);
	Add_KeyFrame(55, 998);

	

	return S_OK;
}

void CState_Combat_Attack_HorizontalDown_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(m_eBounceState);

	pOwner->On_Attack(this);

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE eRandState = Enter_RandomState(pOwner, pAnimator);

	if (eRandState != STATE_END)
		return eRandState;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalDown_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_HorizontalDown_Fiona::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

