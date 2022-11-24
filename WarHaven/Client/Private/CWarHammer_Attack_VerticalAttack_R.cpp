#include "stdafx.h"
#include "CWarHammer_Attack_VerticalAttack_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_Attack_VerticalAttack_R::CWarHammer_Attack_VerticalAttack_R()
{
}

CWarHammer_Attack_VerticalAttack_R::~CWarHammer_Attack_VerticalAttack_R()
{
}

CWarHammer_Attack_VerticalAttack_R* CWarHammer_Attack_VerticalAttack_R::Create()
{
	CWarHammer_Attack_VerticalAttack_R* pInstance = new CWarHammer_Attack_VerticalAttack_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_Attack_VerticalAttack_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_Attack_VerticalAttack_R::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_VERTICALATTACK_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	/* Setting for Blendable */

	m_eAnimLeftorRight = ANIM_BASE_R;
	m_iIdle_Index = 14;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);

	m_eWalkState = STATE_WALK_WARHAMMER_L;
	m_eJumpState = STATE_JUMP_WARHAMMER_L;
	m_eLandState = STATE_JUMP_LAND_WARHAMMER_L;
	m_eFallState = STATE_JUMPFALL_WARHAMMER_L;
	m_eRunState = STATE_RUNBEGIN_WARHAMMER_L;
	m_eIdleState = STATE_IDLE_WARHAMMER_L;
	m_eBounceState = STATE_BOUNCE_WARHAMMER_L;

	m_fDamagePumping = 4.2f;

	return __super::Initialize();
}

void CWarHammer_Attack_VerticalAttack_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_WARHAMMER_R);


	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.5f;
	}
	else
		m_fAnimSpeed = 2.7f;


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_Attack_VerticalAttack_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_Attack_VerticalAttack_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarHammer_Attack_VerticalAttack_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarHammer_Attack_VerticalAttack_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
