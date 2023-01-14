#include "stdafx.h"
#include "CState_Combat_SkillQ_Begin_Sniping_Qanda.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Qanda.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillQ_Begin_Sniping_Qanda::CState_Combat_SkillQ_Begin_Sniping_Qanda()
{
}

CState_Combat_SkillQ_Begin_Sniping_Qanda::~CState_Combat_SkillQ_Begin_Sniping_Qanda()
{
}

CState_Combat_SkillQ_Begin_Sniping_Qanda* CState_Combat_SkillQ_Begin_Sniping_Qanda::Create()
{
	CState_Combat_SkillQ_Begin_Sniping_Qanda* pInstance = new CState_Combat_SkillQ_Begin_Sniping_Qanda();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Begin_Sniping_Qanda");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_Begin_Sniping_Qanda::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_BEGIN_SNIPING_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	return S_OK;
}

void CState_Combat_SkillQ_Begin_Sniping_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	/* ColorControl*/
	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = 45; // 프레임 맞춰놓음

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	pOwner->On_Use(CUnit::SKILL3);
	

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Begin_Sniping_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_AIMING_SNIPING_QANDA;


	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Begin_Sniping_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Combat_SkillQ_Begin_Sniping_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillQ_Begin_Sniping_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{


}
