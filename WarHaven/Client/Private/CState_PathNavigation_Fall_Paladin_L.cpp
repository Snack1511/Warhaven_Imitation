#include "stdafx.h"
#include "CState_PathNavigation_Fall_Paladin_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Fall_Paladin_L::CState_PathNavigation_Fall_Paladin_L()
{
}

CState_PathNavigation_Fall_Paladin_L::~CState_PathNavigation_Fall_Paladin_L()
{
}

CState_PathNavigation_Fall_Paladin_L* CState_PathNavigation_Fall_Paladin_L::Create()
{
    CState_PathNavigation_Fall_Paladin_L* pInstance = new CState_PathNavigation_Fall_Paladin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Fall_Paladin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_PathNavigation_Fall_Paladin_L::Initialize()
{
    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_FALL_PALADIN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_eAILandStateType = AI_STATE_COMMON_LAND_PALADIN_L;

    return S_OK;
}

void CState_PathNavigation_Fall_Paladin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Fall_Paladin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Fall_Paladin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Fall_Paladin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
