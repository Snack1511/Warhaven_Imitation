#include "stdafx.h"
#include "CWarHammer_Attack_Sting_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_Attack_Sting_R::CWarHammer_Attack_Sting_R()
{
}

CWarHammer_Attack_Sting_R::~CWarHammer_Attack_Sting_R()
{
}

CWarHammer_Attack_Sting_R* CWarHammer_Attack_Sting_R::Create()
{
	CWarHammer_Attack_Sting_R* pInstance = new CWarHammer_Attack_Sting_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_Attack_Sting_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_Attack_Sting_R::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 17;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_STING_WARHAMMER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);


	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);

	/* Setting for Blendable */

	m_eAnimLeftorRight = ANIM_BASE_R;
	m_iIdle_Index = 14;

	m_eWalkState = STATE_WALK_WARHAMMER_R;
	m_eJumpState = STATE_JUMP_WARHAMMER_R;
	m_eLandState = STATE_JUMP_LAND_WARHAMMER_R;
	m_eFallState = STATE_JUMPFALL_WARHAMMER_R;
	m_eRunState = STATE_RUNBEGIN_WARHAMMER_R;
	m_eIdleState = STATE_IDLE_WARHAMMER_R;
	m_eBounceState = STATE_BOUNCE_WARHAMMER_R;


	return __super::Initialize();
}

void CWarHammer_Attack_Sting_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_Attack_Sting_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_Attack_Sting_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarHammer_Attack_Sting_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarHammer_Attack_Sting_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
