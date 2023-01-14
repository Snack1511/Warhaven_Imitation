#include "stdafx.h"
#include "CState_PathNavigation_Default_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Default_Qanda::CState_PathNavigation_Default_Qanda()
{
}

CState_PathNavigation_Default_Qanda::~CState_PathNavigation_Default_Qanda()
{
}

CState_PathNavigation_Default_Qanda* CState_PathNavigation_Default_Qanda::Create()
{
    CState_PathNavigation_Default_Qanda* pInstance = new CState_PathNavigation_Default_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Default_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Default_Qanda::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

	// 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 8;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 9;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 10;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 11;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 23;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 24;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 25;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 12;

    m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

    m_iStateChangeKeyFrame = 25;

    m_eJumpState = AI_STATE_PATHNAVIGATION_JUMP_QANDA;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_QANDA;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_QANDA;
    m_eSprintBeginState = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_QANDA;

    return S_OK;
}

void CState_PathNavigation_Default_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Default_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Default_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
