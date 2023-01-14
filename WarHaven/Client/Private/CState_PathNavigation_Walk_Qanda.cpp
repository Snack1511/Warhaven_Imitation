#include "stdafx.h"
#include "CState_PathNavigation_Walk_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Walk_Qanda::CState_PathNavigation_Walk_Qanda()
{
}

CState_PathNavigation_Walk_Qanda::~CState_PathNavigation_Walk_Qanda()
{
}

CState_PathNavigation_Walk_Qanda* CState_PathNavigation_Walk_Qanda::Create()
{
    CState_PathNavigation_Walk_Qanda* pInstance = new CState_PathNavigation_Walk_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Walk_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Walk_Qanda::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 19;
    m_eStateType = AI_STATE_PATHNAVIGATION_WALK_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iAINextState = AI_STATE_PATHNAVIGATION_DEFAULT_QANDA;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_QANDA;

    // 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 22;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 23;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 24;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 25;

    m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;


    return S_OK;
}

void CState_PathNavigation_Walk_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = random(0, 2);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand != 0)
        return m_eStateType;
    else
        return m_iAINextState;


    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Walk_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
