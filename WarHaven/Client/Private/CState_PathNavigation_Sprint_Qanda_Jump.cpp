#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Qanda_Jump.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "UsefulHeaders.h"

CState_PathNavigation_Sprint_Qanda_Jump::CState_PathNavigation_Sprint_Qanda_Jump()
{
}

CState_PathNavigation_Sprint_Qanda_Jump::~CState_PathNavigation_Sprint_Qanda_Jump()
{
}

CState_PathNavigation_Sprint_Qanda_Jump* CState_PathNavigation_Sprint_Qanda_Jump::Create()
{
    CState_PathNavigation_Sprint_Qanda_Jump* pInstance = new CState_PathNavigation_Sprint_Qanda_Jump();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Qanda_Jump");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Qanda_Jump::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 35;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTJUMP_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Qanda_Jump::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_iDirectionRand = STATE_DIRECTION_N;

    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower + 0.5f);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda_Jump::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_QANDA;

    if (!pOwner->Is_Air())
        return AI_STATE_PATHNAVIGATION_SPRINTEND_QANDA;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Qanda_Jump::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda_Jump::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
