#include "stdafx.h"
#include "CState_PathNavigation_Walk_Paladin_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Walk_Paladin_L::CState_PathNavigation_Walk_Paladin_L()
{
}

CState_PathNavigation_Walk_Paladin_L::~CState_PathNavigation_Walk_Paladin_L()
{
}

CState_PathNavigation_Walk_Paladin_L* CState_PathNavigation_Walk_Paladin_L::Create()
{
    CState_PathNavigation_Walk_Paladin_L* pInstance = new CState_PathNavigation_Walk_Paladin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Walk_Paladin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Walk_Paladin_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_eStateType = AI_STATE_PATHNAVIGATION_WALK_PALADIN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iAINextState = AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_L;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_PALADIN_L;

    return S_OK;
}

void CState_PathNavigation_Walk_Paladin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Paladin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Paladin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Walk_Paladin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
