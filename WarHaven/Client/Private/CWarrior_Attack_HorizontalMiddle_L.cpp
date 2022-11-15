#include "stdafx.h"
#include "CWarrior_Attack_HorizontalMiddle_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarrior_Attack_HorizontalMiddle_L::CWarrior_Attack_HorizontalMiddle_L()
{
}

CWarrior_Attack_HorizontalMiddle_L::~CWarrior_Attack_HorizontalMiddle_L()
{
}

CWarrior_Attack_HorizontalMiddle_L* CWarrior_Attack_HorizontalMiddle_L::Create()
{
	CWarrior_Attack_HorizontalMiddle_L* pInstance = new CWarrior_Attack_HorizontalMiddle_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_HorizontalMiddle_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarrior_Attack_HorizontalMiddle_L::Initialize()
{

	__super::Initialize();

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 8;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	//m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
	//m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_BOUNCE_PLAYER_L);



	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iStopIndex = 30;
	m_iIdle_Index = 3;
	m_iLandIndex = 9;
	m_iJumpFallIndex = 1;

	m_iRunAnimIndex[STATE_DIRECTION_E] = 18;
	m_iRunAnimIndex[STATE_DIRECTION_N] = 19;
	m_iRunAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iRunAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iRunAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunAnimIndex[STATE_DIRECTION_W] = 22;

	m_iWalkAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkAnimIndex[STATE_DIRECTION_W] = 37;
	m_iWalkAnimIndex[STATE_DIRECTION_E] = 30;

	m_iJumpAnimIndex[STATE_DIRECTION_N] = 6;
	m_iJumpAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpAnimIndex[STATE_DIRECTION_W] = 8;
	m_iJumpAnimIndex[STATE_DIRECTION_E] = 5;
	m_iJumpAnimIndex[STATE_DIRECTION_NW] = 4; // 제자리

	m_iJumpAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpAnimIndex[STATE_DIRECTION_SE] = 99;

	m_eWalkState = STATE_WALK_PLAYER_R;
	m_eJumpState = STATE_JUMP_PLAYER_R;
	m_eLandState = STATE_JUMP_LAND_PLAYER_R;
	m_eFallState = STATE_JUMPFALL_PLAYER_R;
	m_eRunState = STATE_RUN_PLAYER_R;
	m_eIdleState = STATE_IDLE_PLAYER_R;


	return S_OK;
}

void CWarrior_Attack_HorizontalMiddle_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_HorizontalMiddle_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bAttackTrigger)
	{
		// 공격 진입
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_PLAYER_L;

	}

	return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalMiddle_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CWarrior_Attack_HorizontalMiddle_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalMiddle_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
