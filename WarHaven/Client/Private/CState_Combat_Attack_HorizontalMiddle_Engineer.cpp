#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalMiddle_Engineer.h"

#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"

#include "CEffects_Factory.h"

CState_Combat_Attack_HorizontalMiddle_Engineer::CState_Combat_Attack_HorizontalMiddle_Engineer()
{
}

CState_Combat_Attack_HorizontalMiddle_Engineer::~CState_Combat_Attack_HorizontalMiddle_Engineer()
{
}

HRESULT CState_Combat_Attack_HorizontalMiddle_Engineer::Initialize()
{
	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 110;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;


	Add_KeyFrame(58, 1);
	Add_KeyFrame(80, 2);
	Add_KeyFrame(69, 999);

	return __super::Initialize();
}

void CState_Combat_Attack_HorizontalMiddle_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HammerFlare_1"), pOwner,
		pOwner->Get_Transform()->Get_World(WORLD_POS)); 

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		return m_iAINextState;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalMiddle_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_Attack_HorizontalMiddle_Engineer::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


}

