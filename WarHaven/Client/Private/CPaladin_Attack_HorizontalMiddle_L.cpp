#include "stdafx.h"
#include "CPaladin_Attack_HorizontalMiddle_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_Attack_HorizontalMiddle_L::CPaladin_Attack_HorizontalMiddle_L()
{
}

CPaladin_Attack_HorizontalMiddle_L::~CPaladin_Attack_HorizontalMiddle_L()
{
}

CPaladin_Attack_HorizontalMiddle_L* CPaladin_Attack_HorizontalMiddle_L::Create()
{
	CPaladin_Attack_HorizontalMiddle_L* pInstance = new CPaladin_Attack_HorizontalMiddle_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_Attack_HorizontalMiddle_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_Attack_HorizontalMiddle_L::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 8;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_PALADIN);

	m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
	m_vecAdjState.push_back(STATE_JUMP_PALADIN_R);
	m_vecAdjState.push_back(STATE_RUN_PALADIN_R);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);

	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;

	m_eWalkState = STATE_WALK_PALADIN_R;
	m_eJumpState = STATE_JUMP_PALADIN_R;
	m_eLandState = STATE_JUMP_LAND_PALADIN_R;
	m_eFallState = STATE_JUMPFALL_PALADIN_R;
	m_eRunState = STATE_RUNBEGIN_PALADIN_R;
	m_eIdleState = STATE_IDLE_PALADIN_R;
	m_eBounceState = STATE_BOUNCE_PALADIN_L;

	m_fDamagePumping = 1.5f;

	return __super::Initialize();
}

void CPaladin_Attack_HorizontalMiddle_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_PALADIN_L);


	if (ePrevType == STATE_SWITCH_R_TO_L_PALADIN)
	{
		m_fAnimSpeed = 2.5f;
	}
	else
		m_fAnimSpeed = 2.7f;


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_Attack_HorizontalMiddle_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_Attack_HorizontalMiddle_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CPaladin_Attack_HorizontalMiddle_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CPaladin_Attack_HorizontalMiddle_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
