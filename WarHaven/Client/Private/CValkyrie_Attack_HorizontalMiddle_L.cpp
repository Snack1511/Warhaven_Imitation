#include "stdafx.h"
#include "CValkyrie_Attack_HorizontalMiddle_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CValkyrie_Attack_HorizontalMiddle_L::CValkyrie_Attack_HorizontalMiddle_L()
{
}

CValkyrie_Attack_HorizontalMiddle_L::~CValkyrie_Attack_HorizontalMiddle_L()
{
}

CValkyrie_Attack_HorizontalMiddle_L* CValkyrie_Attack_HorizontalMiddle_L::Create()
{
	CValkyrie_Attack_HorizontalMiddle_L* pInstance = new CValkyrie_Attack_HorizontalMiddle_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CValkyrie_Attack_HorizontalMiddle_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CValkyrie_Attack_HorizontalMiddle_L::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 8;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);


	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;

	m_eWalkState = STATE_WALK_VALKYRIE_R;
	m_eJumpState = STATE_JUMP_VALKYRIE_R;
	m_eLandState = STATE_JUMP_LAND_VALKYRIE_R;
	m_eFallState = STATE_JUMPFALL_VALKYRIE_R;
	m_eRunState = STATE_RUN_VALKYRIE_R;
	m_eIdleState = STATE_IDLE_VALKYRIE_R;
	m_eBounceState = STATE_BOUNCE_VALKYRIE_L;

	return __super::Initialize();
}

void CValkyrie_Attack_HorizontalMiddle_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	pOwner->Set_BounceState(STATE_BOUNCE_VALKYRIE_L);

	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


	/* Owner�� Animator Set Idle�� */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CValkyrie_Attack_HorizontalMiddle_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_Attack_HorizontalMiddle_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CValkyrie_Attack_HorizontalMiddle_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CValkyrie_Attack_HorizontalMiddle_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
