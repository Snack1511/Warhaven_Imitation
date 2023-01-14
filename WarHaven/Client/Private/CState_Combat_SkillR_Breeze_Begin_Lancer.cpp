#include "stdafx.h"
#include "CState_Combat_SkillR_Breeze_Begin_Lancer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Lancer.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"
#include "CLancerNeedle.h"


CState_Combat_SkillR_Breeze_Begin_Lancer::CState_Combat_SkillR_Breeze_Begin_Lancer()
{
}

CState_Combat_SkillR_Breeze_Begin_Lancer::~CState_Combat_SkillR_Breeze_Begin_Lancer()
{
}

CState_Combat_SkillR_Breeze_Begin_Lancer* CState_Combat_SkillR_Breeze_Begin_Lancer::Create()
{
	CState_Combat_SkillR_Breeze_Begin_Lancer* pInstance = new CState_Combat_SkillR_Breeze_Begin_Lancer();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillR_Breeze_Begin_Lancer");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillR_Breeze_Begin_Lancer::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bSting = true;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_BREEZE_BEGIN_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_fDamagePumping = 15.f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	Add_KeyFrame(3, 0);


	// return __super::Initialize();
	return S_OK;
}

void CState_Combat_SkillR_Breeze_Begin_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed * 1.5f;

	m_fDamagePumping = 15.f;
	pOwner->Get_Status().fDamageMultiplier = m_fDamagePumping;

	
	pOwner->TurnOn_TrailEffect(true);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_RUSH);

	for (_int i = 0; i < CUnit_Lancer::eNeedle::eNeedle_Max; ++i)
	{
		CLancerNeedle* pNeedle = static_cast<CUnit_Lancer*>(pOwner)->Get_Needle(i);

		if (!pNeedle)
			continue;

		pNeedle->On_ChangePhase(CLancerNeedle::LANCERNEEDLE_ATTACKBEGIN);
	}

	__super::Set_StraightLook(pOwner);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillR_Breeze_Begin_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_BREEZE_LOOP_LANCER;

	//Wall_Detection(pOwner);
	Straight_Move(pOwner);

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillR_Breeze_Begin_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->TurnOn_TrailEffect(false);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_LANCER);
}

STATE_TYPE CState_Combat_SkillR_Breeze_Begin_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillR_Breeze_Begin_Lancer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Physics_Setting(m_fMaxSpeed, pOwner);
		break;

	default:
		break;
	}


}
