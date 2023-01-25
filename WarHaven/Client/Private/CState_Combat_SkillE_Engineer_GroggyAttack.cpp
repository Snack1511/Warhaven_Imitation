#include "stdafx.h"
#include "CState_Combat_SkillE_Engineer_GroggyAttack.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillE_Engineer_GroggyAttack::CState_Combat_SkillE_Engineer_GroggyAttack()
{
}

CState_Combat_SkillE_Engineer_GroggyAttack::~CState_Combat_SkillE_Engineer_GroggyAttack()
{
}

CState_Combat_SkillE_Engineer_GroggyAttack* CState_Combat_SkillE_Engineer_GroggyAttack::Create()
{
	CState_Combat_SkillE_Engineer_GroggyAttack* pInstance = new CState_Combat_SkillE_Engineer_GroggyAttack();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillE_Engineer_GroggyAttack");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillE_Engineer_GroggyAttack::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 13;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GROGGYATTACK_ENGINEER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 60;


	Add_KeyFrame(30, 0);
	Add_KeyFrame(32, 1);
	Add_KeyFrame(59, 2);

	return S_OK;
}

void CState_Combat_SkillE_Engineer_GroggyAttack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL2);

	m_iDirectionRand = STATE_DIRECTION_N;

	m_fMaxSpeed = pOwner->Get_Status().fDashAttackSpeed;

	CColorController::COLORDESC tColorDesc;

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 0.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = 32; // 프레임 맞춰놓음

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);
	
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE_Engineer_GroggyAttack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R;

	DoMove_AI(pOwner, pAnimator);


	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE_Engineer_GroggyAttack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GroggyCollider(false);
}

STATE_TYPE CState_Combat_SkillE_Engineer_GroggyAttack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillE_Engineer_GroggyAttack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Physics_Setting_AI(m_fMaxSpeed, pOwner);
		break;

	case 1:
		pOwner->Enable_GroggyCollider(true);
		Play_Voice(pOwner, L"Voice_GroggyAttack", 1.f);
		Play_Sound(L"Effect_GroryBegin");
		break;

	case 2:
		pOwner->Enable_GroggyCollider(false);
		break;


	default:
		break;
	}

	
	

}
