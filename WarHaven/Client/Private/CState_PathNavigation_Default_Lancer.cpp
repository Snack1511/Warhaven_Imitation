#include "stdafx.h"
#include "CState_PathNavigation_Default_Lancer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Default_Lancer::CState_PathNavigation_Default_Lancer()
{
}

CState_PathNavigation_Default_Lancer::~CState_PathNavigation_Default_Lancer()
{
}

CState_PathNavigation_Default_Lancer* CState_PathNavigation_Default_Lancer::Create()
{
    CState_PathNavigation_Default_Lancer* pInstance = new CState_PathNavigation_Default_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Default_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Default_Lancer::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

	// 알파벳 순 애니메이션 정렬

    m_eJumpState = AI_STATE_PATHNAVIGATION_JUMP_LANCER;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_LANCER;
    m_eWalkState = AI_STATE_PATHNAVIGATION_DEFAULT_LANCER;
    m_eSprintBeginState = AI_STATE_PATHNAVIGATION_DEFAULT_LANCER;

    return __super::Initialize();
}

void CState_PathNavigation_Default_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Default_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Default_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Lancer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
