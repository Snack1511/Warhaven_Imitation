#include "stdafx.h"
#include "CState_PathNavigation_Run_Engineer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Run_Engineer_R::CState_PathNavigation_Run_Engineer_R()
{
}

CState_PathNavigation_Run_Engineer_R::~CState_PathNavigation_Run_Engineer_R()
{
}

CState_PathNavigation_Run_Engineer_R* CState_PathNavigation_Run_Engineer_R::Create()
{
    CState_PathNavigation_Run_Engineer_R* pInstance = new CState_PathNavigation_Run_Engineer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Run_Engineer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Run_Engineer_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 27;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_ENGINEER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;


    m_fAnimSpeed = 2.5f;

	// 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 29;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 45;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 46;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 47;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 33;

    m_eJumpState = AI_STATE_PATHNAVIGATION_JUMP_ENGINEER_R;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_ENGINEER_R;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_ENGINEER_R;

    return S_OK;
}

void CState_PathNavigation_Run_Engineer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Run_Engineer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Run_Engineer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Run_Engineer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
