#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Engineer_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Engineer_Begin::CState_PathNavigation_Sprint_Engineer_Begin()
{
}

CState_PathNavigation_Sprint_Engineer_Begin::~CState_PathNavigation_Sprint_Engineer_Begin()
{
}

CState_PathNavigation_Sprint_Engineer_Begin* CState_PathNavigation_Sprint_Engineer_Begin::Create()
{
    CState_PathNavigation_Sprint_Engineer_Begin* pInstance = new CState_PathNavigation_Sprint_Engineer_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Engineer_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Engineer_Begin::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 56;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_ENGINEER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    m_fInterPolationTime = 0.1f;


    Add_KeyFrame(10, 0, true);
    Add_KeyFrame(20, 1, true);
    Add_KeyFrame(51, 1, true);
    Add_KeyFrame(79, 1, true);
    Add_KeyFrame(103, 1, true);
    return __super::Initialize();
}

void CState_PathNavigation_Sprint_Engineer_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Engineer_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_SPRINTLOOP_ENGINEER;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Engineer_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Engineer_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
