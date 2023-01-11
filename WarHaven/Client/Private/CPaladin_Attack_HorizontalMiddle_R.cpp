#include "stdafx.h"
#include "CPaladin_Attack_HorizontalMiddle_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_Attack_HorizontalMiddle_R::CPaladin_Attack_HorizontalMiddle_R()
{
}

CPaladin_Attack_HorizontalMiddle_R::~CPaladin_Attack_HorizontalMiddle_R()
{
}

CPaladin_Attack_HorizontalMiddle_R* CPaladin_Attack_HorizontalMiddle_R::Create()
{
	CPaladin_Attack_HorizontalMiddle_R* pInstance = new CPaladin_Attack_HorizontalMiddle_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_Attack_HorizontalMiddle_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_Attack_HorizontalMiddle_R::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_PALADIN);

	m_vecAdjState.push_back(STATE_WALK_PALADIN_L);
	m_vecAdjState.push_back(STATE_JUMP_PALADIN_L);
	m_vecAdjState.push_back(STATE_RUN_PALADIN_L);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);

	/* Setting for Blendable */

	m_eAnimLeftorRight = ANIM_BASE_R;


	m_eWalkState = STATE_WALK_PALADIN_L;
	m_eJumpState = STATE_JUMP_PALADIN_L;
	m_eLandState = STATE_JUMP_LAND_PALADIN_L;
	m_eFallState = STATE_JUMPFALL_PALADIN_L;
	m_eRunState = STATE_RUN_PALADIN_L;
	m_eIdleState = STATE_IDLE_PALADIN_L;
	m_eBounceState = STATE_BOUNCE_PALADIN_R;
	
	m_fDamagePumping = 1.5f;

	return __super::Initialize();
}

void CPaladin_Attack_HorizontalMiddle_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_PALADIN_R);


	if (ePrevType == STATE_SWITCH_L_TO_R_PALADIN)
	{
		m_fAnimSpeed = 2.5f;
	}
	else
		m_fAnimSpeed = 2.7f;


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_Attack_HorizontalMiddle_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_Attack_HorizontalMiddle_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CPaladin_Attack_HorizontalMiddle_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

}

void CPaladin_Attack_HorizontalMiddle_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
