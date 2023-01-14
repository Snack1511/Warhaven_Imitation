#include "stdafx.h"
#include "CState_Combat_SkillR_Breeze_Loop_Lancer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Lancer.h"
#include "CLancerNeedle.h"

#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillR_Breeze_Loop_Lancer::CState_Combat_SkillR_Breeze_Loop_Lancer()
{
}

CState_Combat_SkillR_Breeze_Loop_Lancer::~CState_Combat_SkillR_Breeze_Loop_Lancer()
{
}

CState_Combat_SkillR_Breeze_Loop_Lancer* CState_Combat_SkillR_Breeze_Loop_Lancer::Create()
{
	CState_Combat_SkillR_Breeze_Loop_Lancer* pInstance = new CState_Combat_SkillR_Breeze_Loop_Lancer();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillR_Breeze_Loop_Lancer");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillR_Breeze_Loop_Lancer::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bSting = true;

	m_fDamagePumping = 15.f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_BREEZE_LOOP_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	return S_OK;
}

void CState_Combat_SkillR_Breeze_Loop_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fDamagePumping = 15.f;
	pOwner->Get_Status().fDamageMultiplier = m_fDamagePumping;

	m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed * 1.5f;
	Physics_Setting(m_fMaxSpeed, pOwner);

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON_L;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	pOwner->TurnOn_TrailEffect(true);
	pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, true);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_BREEZE);

	m_fMaxTime = frandom(3.f, 4.9f);

	__super::Set_StraightLook(pOwner);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillR_Breeze_Loop_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fTimeAcc += fDT(0);

	if (m_fTimeAcc > m_fMaxTime)
		return AI_STATE_COMBAT_STOP_LANCER;

	Wall_Detection(pOwner);
	Straight_Move(pOwner);

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillR_Breeze_Loop_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CUnit_Lancer*>(pOwner)->Reset_NeedleNums();

	for (_int i = 0; i < CUnit_Lancer::eNeedle::eNeedle_Max; ++i)
	{
		CLancerNeedle* pNeedle = static_cast<CUnit_Lancer*>(pOwner)->Get_Needle(i);

		if (!pNeedle)
			continue;

		pNeedle->On_ChangePhase(CLancerNeedle::LANCERNEEDLE_STOP);
		pNeedle->Enable_Needle(false);
	}


	pOwner->TurnOn_TrailEffect(false);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_LANCER);
	pOwner->On_Use(CUnit::SKILL1);
	pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
}

STATE_TYPE CState_Combat_SkillR_Breeze_Loop_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}
