#include "stdafx.h"
#include "CState_PathNavigation_Default_Fiona_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Default_Fiona_L::CState_PathNavigation_Default_Fiona_L()
{
}

CState_PathNavigation_Default_Fiona_L::~CState_PathNavigation_Default_Fiona_L()
{
}

CState_PathNavigation_Default_Fiona_L* CState_PathNavigation_Default_Fiona_L::Create()
{
    CState_PathNavigation_Default_Fiona_L* pInstance = new CState_PathNavigation_Default_Fiona_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Default_Fiona_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Default_Fiona_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_FIONA_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;

    m_eJumpState = AI_STATE_PATHNAVIGATION_JUMP_FIONA_L;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_FIONA_L;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_FIONA_L;

    return S_OK;
}

void CState_PathNavigation_Default_Fiona_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Default_Fiona_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Fiona_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Default_Fiona_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
