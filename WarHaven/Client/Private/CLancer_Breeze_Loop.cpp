#include "stdafx.h"
#include "CLancer_Breeze_Loop.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Lancer.h"
#include "CLancerNeedle.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CLancer_Breeze_Loop::CLancer_Breeze_Loop()
{
}

CLancer_Breeze_Loop::~CLancer_Breeze_Loop()
{
}

CLancer_Breeze_Loop* CLancer_Breeze_Loop::Create()
{
	CLancer_Breeze_Loop* pInstance = new CLancer_Breeze_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CLancer_Breeze_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CLancer_Breeze_Loop::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bSting = true;

	m_fDamagePumping = 15.f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_BREEZE_LOOP_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	Add_KeyFrame(3, 0, true);

	return S_OK;
}

void CLancer_Breeze_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fDamagePumping = 15.f;
	pOwner->Get_Status().fDamageMultiplier = m_fDamagePumping;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

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

	GAMEINSTANCE->Start_RadialBlur(0.015f);

	pOwner->TurnOn_TrailEffect(true);
	pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, true);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_BREEZE);

	CUser::Get_Instance()->Disable_LancerGauge();
	CUser::Get_Instance()->SetActive_Gauge(true);


	for (int i = 0; i < 4; ++i)
		CUser::Get_Instance()->Set_LancerGauge(i, 0.f, 1.f);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CLancer_Breeze_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fTimeAcc += fDT(0);

	CUser::Get_Instance()->Set_BreezeTime(m_fTimeAcc, 5.f);

	if (m_fTimeAcc > 5.f || KEY(RBUTTON, AWAY))
		return STATE_STOP_LANCER;

	pOwner->Get_PhysicsCom()->Set_Accel(m_fMyAccel);
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();
	return __super::Tick(pOwner, pAnimator);
}

void CLancer_Breeze_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CUnit_Lancer*>(pOwner)->Reset_NeedleNums();

	for (_int i = 0; i < CUnit_Lancer::eNeedle::eNeedle_Max; ++i)
	{
		CLancerNeedle* pNeedle = static_cast<CUnit_Lancer*>(pOwner)->Get_Needle(i);

		if (!pNeedle)
			continue;

		pNeedle->On_ChangePhase(CLancerNeedle::LANCERNEEDLE_STOP);
	}
	CUser::Get_Instance()->SetActive_Gauge(false);

	pOwner->TurnOn_TrailEffect(false);
	GAMEINSTANCE->Stop_RadialBlur();

	CFunctor::Play_Sound(L"Effect_LanceDisable", CHANNEL_EFFECTS);

	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_LANCER);
	pOwner->On_Use(CUnit::SKILL1);
	pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
}

STATE_TYPE CLancer_Breeze_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* LANCER가 Loop로 오는 조건
	1.  Begin 이 끝났을 경우.
	*/
	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CLancer_Breeze_Loop::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepHorse", CHANNEL_ENVIRONMENT);
		break;

	default:
		break;
	}


}