#include "stdafx.h"
#include "CWarHammer_AirSpike_Begin.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_AirSpike_Begin::CWarHammer_AirSpike_Begin()
{
}

CWarHammer_AirSpike_Begin::~CWarHammer_AirSpike_Begin()
{
}

CWarHammer_AirSpike_Begin* CWarHammer_AirSpike_Begin::Create()
{
	CWarHammer_AirSpike_Begin* pInstance = new CWarHammer_AirSpike_Begin();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_AirSpike_Begin");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_AirSpike_Begin::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_AIRSPIKE_BEGIN_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_AIRSPIKE_LOOP_WARHAMMER);


	Add_KeyFrame(3, 0);


	// return __super::Initialize();
	return S_OK;
}

void CWarHammer_AirSpike_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL3);

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fSprintJumpSpeed;



	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc;

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	GAMEINSTANCE->Start_RadialBlur(0.015f);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Sound(L"Effect_AirStrikeUp_Warhammer");
}

STATE_TYPE CWarHammer_AirSpike_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Set_Accel(m_fMyAccel);
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();
	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_AirSpike_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	GAMEINSTANCE->Stop_RadialBlur();
}

STATE_TYPE CWarHammer_AirSpike_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (!pOwner->Can_Use(CUnit::SKILL3))
		return STATE_END;

	if (KEY(Q, HOLD))
		return m_eStateType;


	return STATE_END;
}

void CWarHammer_AirSpike_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);
		Physics_Setting(m_fMaxSpeed, pOwner);
		Play_Voice(pOwner, L"Voice_AirSpike", 1.f);
		break;

	default:
		break;
	}


}
