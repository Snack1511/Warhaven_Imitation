#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Priest_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Priest_Begin::CState_PathNavigation_Sprint_Priest_Begin()
{
}

CState_PathNavigation_Sprint_Priest_Begin::~CState_PathNavigation_Sprint_Priest_Begin()
{
}

CState_PathNavigation_Sprint_Priest_Begin* CState_PathNavigation_Sprint_Priest_Begin::Create()
{
    CState_PathNavigation_Sprint_Priest_Begin* pInstance = new CState_PathNavigation_Sprint_Priest_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Priest_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Priest_Begin::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 43;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    Add_KeyFrame(4, 1, true);
    Add_KeyFrame(27, 1, true);
    Add_KeyFrame(39, 1, true);
    Add_KeyFrame(68, 1, true);
    Add_KeyFrame(90, 1, true);
    return S_OK;
}

void CState_PathNavigation_Sprint_Priest_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Priest_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_SPRINTLOOP_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Priest_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Priest_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
