#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Warrior_Fall.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Warrior_Fall::CState_PathNavigation_Sprint_Warrior_Fall()
{
}

CState_PathNavigation_Sprint_Warrior_Fall::~CState_PathNavigation_Sprint_Warrior_Fall()
{
}

CState_PathNavigation_Sprint_Warrior_Fall* CState_PathNavigation_Sprint_Warrior_Fall::Create()
{
    CState_PathNavigation_Sprint_Warrior_Fall* pInstance = new CState_PathNavigation_Sprint_Warrior_Fall();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Warrior_Fall");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Warrior_Fall::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 57;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Warrior_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_Fall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_SPRINTLOOP_WARRIOR;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Warrior_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
