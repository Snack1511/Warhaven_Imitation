#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Fiona_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Fiona_Begin::CState_PathNavigation_Sprint_Fiona_Begin()
{
}

CState_PathNavigation_Sprint_Fiona_Begin::~CState_PathNavigation_Sprint_Fiona_Begin()
{
}

CState_PathNavigation_Sprint_Fiona_Begin* CState_PathNavigation_Sprint_Fiona_Begin::Create()
{
    CState_PathNavigation_Sprint_Fiona_Begin* pInstance = new CState_PathNavigation_Sprint_Fiona_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Fiona_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Fiona_Begin::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 45;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_FIONA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;


    Add_KeyFrame(10, 0);
    Add_KeyFrame(25, 1);
    Add_KeyFrame(44, 1);
    Add_KeyFrame(60, 1);
    Add_KeyFrame(77, 1);
    Add_KeyFrame(94, 1);
    Add_KeyFrame(113, 1);
    return __super::Initialize();
}

void CState_PathNavigation_Sprint_Fiona_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Fiona_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_SPRINTLOOP_FIONA;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Fiona_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Fiona_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
