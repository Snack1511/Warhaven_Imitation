#include "stdafx.h"
#include "CGuard_Cancel_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Cancel_Qanda::CGuard_Cancel_Qanda()
{
}

CGuard_Cancel_Qanda::~CGuard_Cancel_Qanda()
{
}

CGuard_Cancel_Qanda* CGuard_Cancel_Qanda::Create()
{
    CGuard_Cancel_Qanda* pInstance = new CGuard_Cancel_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Cancel_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Cancel_Qanda::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_eAnimType = ANIM_ATTACK;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_CANCEL_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 75;

    m_vecAdjState.push_back(STATE_IDLE_QANDA);
    m_vecAdjState.push_back(STATE_WALK_QANDA);
    m_vecAdjState.push_back(STATE_RUN_QANDA);

    m_vecAdjState.push_back(STATE_GUARD_QANDA);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_QANDA);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_QANDA);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_QANDA);

	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;

	m_iIdle_Index = 1;
	m_iLandRightIndex = 7;
	m_iLandLeftIndex = 99;
	m_iJumpFallRightIndex = 0;
	m_iJumpFallLeftIndex = 99;

	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 99;

	
	
	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 8;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 9;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 10;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 11;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 22;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 23;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 24;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 12;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 18;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 19;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 22;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 23;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 24;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 25;

	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 99;

	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 3;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 4;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 5;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 6;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 2;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;


	m_eWalkState = STATE_WALK_QANDA;
	m_eJumpState = STATE_JUMP_QANDA;
	m_eLandState = STATE_WALK_QANDA;
	m_eFallState = STATE_JUMPFALL_QANDA;
	m_eRunState = STATE_WALK_QANDA;
	m_eIdleState = STATE_IDLE_QANDA;
	m_eBounceState = STATE_WALK_QANDA;

	m_bSmootMoveLoop = true;

    return __super::Initialize();
}

void CGuard_Cancel_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed * 0.7f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed * 0.7f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Cancel_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Cancel_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CGuard_Cancel_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
