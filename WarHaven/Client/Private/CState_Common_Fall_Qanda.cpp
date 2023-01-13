#include "stdafx.h"
#include "CState_Common_Fall_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"

CState_Common_Fall_Qanda::CState_Common_Fall_Qanda()
{
}

CState_Common_Fall_Qanda::~CState_Common_Fall_Qanda()
{
}


HRESULT CState_Common_Fall_Qanda::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_FALL_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iAINextState = AI_STATE_COMMON_LAND_QANDA;

    m_iStateChangeKeyFrame = 99;

    m_fInterPolationTime = 0.15f;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

CState_Common_Fall_Qanda* CState_Common_Fall_Qanda::Create()
{
    CState_Common_Fall_Qanda* pInstance = new CState_Common_Fall_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Fall_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CState_Common_Fall_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Fall_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Fall_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Fall_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
