#include "stdafx.h"
#include "CState_PathNavigation_Walk_Paladin_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Walk_Paladin_R::CState_PathNavigation_Walk_Paladin_R()
{
}

CState_PathNavigation_Walk_Paladin_R::~CState_PathNavigation_Walk_Paladin_R()
{
}

CState_PathNavigation_Walk_Paladin_R* CState_PathNavigation_Walk_Paladin_R::Create()
{
    CState_PathNavigation_Walk_Paladin_R* pInstance = new CState_PathNavigation_Walk_Paladin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Walk_Paladin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Walk_Paladin_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_eStateType = AI_STATE_PATHNAVIGATION_WALK_PALADIN_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iAINextState = AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_R;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_PALADIN_R;


    return S_OK;
}

void CState_PathNavigation_Walk_Paladin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Paladin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Paladin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Walk_Paladin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
