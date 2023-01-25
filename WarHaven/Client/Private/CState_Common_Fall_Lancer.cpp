#include "stdafx.h"
#include "CState_Common_Fall_Lancer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"

CState_Common_Fall_Lancer::CState_Common_Fall_Lancer()
{
}

CState_Common_Fall_Lancer::~CState_Common_Fall_Lancer()
{
}


HRESULT CState_Common_Fall_Lancer::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_FALL_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iAINextState = AI_STATE_COMMON_LAND_LANCER;

    m_iStateChangeKeyFrame = 99;

    m_fInterPolationTime = 0.15f;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

CState_Common_Fall_Lancer* CState_Common_Fall_Lancer::Create()
{
    CState_Common_Fall_Lancer* pInstance = new CState_Common_Fall_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Fall_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CState_Common_Fall_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Fall_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Fall_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Fall_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
