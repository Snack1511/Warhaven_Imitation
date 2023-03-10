#include "stdafx.h"
#include "CState_PathNavigation_Walk_Warrior_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Walk_Warrior_L::CState_PathNavigation_Walk_Warrior_L()
{
}

CState_PathNavigation_Walk_Warrior_L::~CState_PathNavigation_Walk_Warrior_L()
{
}

CState_PathNavigation_Walk_Warrior_L* CState_PathNavigation_Walk_Warrior_L::Create()
{
    CState_PathNavigation_Walk_Warrior_L* pInstance = new CState_PathNavigation_Walk_Warrior_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Walk_Warrior_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Walk_Warrior_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_WALK_WARRIOR_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 30;


    m_iAINextState = AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_L;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_WARRIOR_L;

    return S_OK;
}

void CState_PathNavigation_Walk_Warrior_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Warrior_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Warrior_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Walk_Warrior_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
