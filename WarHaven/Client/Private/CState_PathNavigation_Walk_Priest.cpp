#include "stdafx.h"
#include "CState_PathNavigation_Walk_Priest.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Walk_Priest::CState_PathNavigation_Walk_Priest()
{
}

CState_PathNavigation_Walk_Priest::~CState_PathNavigation_Walk_Priest()
{
}

CState_PathNavigation_Walk_Priest* CState_PathNavigation_Walk_Priest::Create()
{
    CState_PathNavigation_Walk_Priest* pInstance = new CState_PathNavigation_Walk_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Walk_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Walk_Priest::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 31;
    m_eStateType = AI_STATE_PATHNAVIGATION_WALK_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iAINextState = AI_STATE_PATHNAVIGATION_DEFAULT_PRIEST;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_PRIEST;

    // 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;

    m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    Add_KeyFrame(15, 0, true);
    Add_KeyFrame(50, 0, true);

    return S_OK;
}

void CState_PathNavigation_Walk_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));
    m_iRand = random(0, 2);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand != 0)
        return m_eStateType;
    else
        return m_iAINextState;


    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Walk_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
