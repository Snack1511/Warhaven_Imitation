#include "stdafx.h"
#include "CState_Patrol_Walk_Archer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk_Archer_R::CState_Patrol_Walk_Archer_R()
{
}

CState_Patrol_Walk_Archer_R::~CState_Patrol_Walk_Archer_R()
{
}

CState_Patrol_Walk_Archer_R* CState_Patrol_Walk_Archer_R::Create()
{
    CState_Patrol_Walk_Archer_R* pInstance = new CState_Patrol_Walk_Archer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Walk_Archer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Walk_Archer_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 39;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_WALK_ARCHER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 38;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 45;

    m_eJumpFallStateType = AI_STATE_COMMON_LAND_ARCHER_R;

    return __super::Initialize();
}

void CState_Patrol_Walk_Archer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk_Archer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_iRand == 0)
            return AI_STATE_PATROL_DEFAULT_ARCHER_R;
        else
            --m_iRand;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Walk_Archer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Walk_Archer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
