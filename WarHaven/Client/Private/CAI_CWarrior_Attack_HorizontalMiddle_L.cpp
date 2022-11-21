#include "stdafx.h"
#include "CAI_CWarrior_Attack_HorizontalMiddle_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CAI_CWarrior_Attack_HorizontalMiddle_L::CAI_CWarrior_Attack_HorizontalMiddle_L()
{
}

CAI_CWarrior_Attack_HorizontalMiddle_L::~CAI_CWarrior_Attack_HorizontalMiddle_L()
{
}

CAI_CWarrior_Attack_HorizontalMiddle_L* CAI_CWarrior_Attack_HorizontalMiddle_L::Create()
{
	CAI_CWarrior_Attack_HorizontalMiddle_L* pInstance = new CAI_CWarrior_Attack_HorizontalMiddle_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CAI_CWarrior_Attack_HorizontalMiddle_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CAI_CWarrior_Attack_HorizontalMiddle_L::Initialize()
{
	 __super::Initialize();

	m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 8;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	
	//m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	//m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_R);

	////m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
	////m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	//m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);



	/* Setting for Blendable */
	//m_eAnimLeftorRight = ANIM_BASE_L;
	//m_iIdle_Index = 3;

	//m_iLandRightIndex = 17;
	//m_iLandLeftIndex = 9;
	//m_iJumpFallRightIndex = 10;
	//m_iJumpFallLeftIndex = 1;


	//m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 18;
	//m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 19;
	//m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 20;
	//m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 21;
	//m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 34;
	//m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	//m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	//m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 22;

	//m_iRunRightAnimIndex[STATE_DIRECTION_E] = 26;
	//m_iRunRightAnimIndex[STATE_DIRECTION_N] = 27;
	//m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 28;
	//m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 29;
	//m_iRunRightAnimIndex[STATE_DIRECTION_S] = 42;
	//m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 43;
	//m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 44;
	//m_iRunRightAnimIndex[STATE_DIRECTION_W] = 30;


	//m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 41;
	//m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 40;
	//m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 39;
	//m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 44;
	//m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 43;
	//m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 42;
	//m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 45;
	//m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 38;


	//m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 33;
	//m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 32;
	//m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 31;
	//m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	//m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	//m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 34;
	//m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 37;
	//m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 30;



	//m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 14;
	//m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 15;
	//m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 16;
	//m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 13;
	//m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12; // 제자리
	//m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	//m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	//m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	//m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 6;
	//m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 7;
	//m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 8;
	//m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 5;
	//m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 6;
	//m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 6;
	//m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 7;
	//m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 7;


	//m_eWalkState = STATE_WALK_PLAYER_R;
	//m_eJumpState = STATE_JUMP_PLAYER_R;
	//m_eLandState = STATE_JUMP_LAND_PLAYER_R;
	//m_eFallState = STATE_JUMPFALL_PLAYER_R;
	//m_eRunState = STATE_RUN_BEGIN_PLAYER_R;
	//m_eIdleState = STATE_IDLE_PLAYER_R;
	//m_eBounceState = STATE_BOUNCE_PLAYER_L;


	return S_OK;

}

void CAI_CWarrior_Attack_HorizontalMiddle_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
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

STATE_TYPE CAI_CWarrior_Attack_HorizontalMiddle_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY;

	return __super::Tick(pOwner, pAnimator);
}

void CAI_CWarrior_Attack_HorizontalMiddle_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CAI_CWarrior_Attack_HorizontalMiddle_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CAI_CWarrior_Attack_HorizontalMiddle_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
