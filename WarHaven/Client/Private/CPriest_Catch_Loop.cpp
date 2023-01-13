#include "stdafx.h"
#include "CPriest_Catch_Loop.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"
#include "CUnit_Priest.h"


CPriest_Catch_Loop::CPriest_Catch_Loop()
{
}

CPriest_Catch_Loop::~CPriest_Catch_Loop()
{
}

CPriest_Catch_Loop* CPriest_Catch_Loop::Create()
{
    CPriest_Catch_Loop* pInstance = new CPriest_Catch_Loop();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CPriest_Catch_Loop");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CPriest_Catch_Loop::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PROJECTILECATCH_LOOP_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.4f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 220;


	m_iStopIndex = 712;
	m_iAttackEndIndex = 712;

	m_vecAdjState.push_back(STATE_IDLE_PRIEST);
	m_vecAdjState.push_back(STATE_WALK_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_PRIEST);


	Init_AttackState_Priest();
	Init_CommonState_Player();

	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 1;
	m_iLandRightIndex = 16;
	m_iLandLeftIndex = 16;
	m_iJumpFallRightIndex = 0;
	m_iJumpFallLeftIndex = 0;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 99;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 17;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 18;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 19;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 20;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 24;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 30;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 37;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 99;



	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 12;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 11; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;


	m_eWalkState = STATE_WALK_PRIEST;
	m_eJumpState = STATE_JUMP_PRIEST;
	m_eLandState = STATE_JUMP_LAND_PRIEST;
	m_eFallState = STATE_JUMPFALL_PRIEST;
	m_eRunState = STATE_WALK_PRIEST;
	m_eIdleState = STATE_IDLE_PRIEST;
	m_eBounceState = STATE_BOUNCE_PRIEST;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	m_bSmootMoveLoop = true;
	m_bLoopAction = false;

	return __super::Initialize();
}

void CPriest_Catch_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	pOwner->Enable_GuardCollider(true);

	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPriest_Catch_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(Q, AWAY))
	{
		if (KEY(W, TAP) || KEY(A, TAP) || KEY(S, TAP) || KEY(D, TAP))
			return STATE_RUN_PRIEST;
		else
			return STATE_IDLE_PRIEST;

		pOwner->On_Use(CUnit::SKILL3);
	}

	if (pOwner->Get_CatchProjectileObject())
		return STATE_PROJECTILECATCH_HIT_PRIEST;

	else if (pOwner->Get_CatchedBall())
		return STATE_PROJECTILECATCH_HIT_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CPriest_Catch_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(false);

	pOwner->Enable_GuardCollider(false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CPriest_Catch_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}


void CPriest_Catch_Loop::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
