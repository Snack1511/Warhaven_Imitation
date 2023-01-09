#include "stdafx.h"
#include "CLancer_Breeze_Begin.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Lancer.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"
#include "CLancerNeedle.h"


CLancer_Breeze_Begin::CLancer_Breeze_Begin()
{
}

CLancer_Breeze_Begin::~CLancer_Breeze_Begin()
{
}

CLancer_Breeze_Begin* CLancer_Breeze_Begin::Create()
{
	CLancer_Breeze_Begin* pInstance = new CLancer_Breeze_Begin();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CLancer_Breeze_Begin");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CLancer_Breeze_Begin::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bSting = true;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_BREEZE_BEGIN_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_fDamagePumping = 9.f;

	m_vecAdjState.push_back(STATE_ATTACK_BREEZE_LOOP_LANCER);


	Add_KeyFrame(3, 0);


	// return __super::Initialize();
	return S_OK;
}

void CLancer_Breeze_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed * 1.5f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	GAMEINSTANCE->Start_RadialBlur(0.02f);
	GAMEINSTANCE->Start_ChromaticAberration(20.f);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_ZOOMOUT);

	for (_int i = 0; i < CUnit_Lancer::eNeedle::eNeedle_Max; ++i)
	{
		CLancerNeedle* pNeedle = static_cast<CUnit_Lancer*>(pOwner)->Get_Needle(i);

		if (!pNeedle)
			continue;

		pNeedle->On_ChangePhase(CLancerNeedle::LANCERNEEDLE_ATTACKBEGIN);
	}

	 

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CLancer_Breeze_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Set_Accel(m_fMyAccel);
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();
	return __super::Tick(pOwner, pAnimator);
}

void CLancer_Breeze_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	GAMEINSTANCE->Stop_RadialBlur();
	GAMEINSTANCE->Stop_ChromaticAberration();
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_ZOOMOUT);
}

STATE_TYPE CLancer_Breeze_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (!pOwner->Can_Use(CUnit::SKILL1))
		return STATE_END;

	if (KEY(RBUTTON, HOLD))
		return m_eStateType;


	return STATE_END;
}

void CLancer_Breeze_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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
