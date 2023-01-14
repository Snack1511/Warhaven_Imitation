#include "stdafx.h"
#include "CState_Patrol_Walk_Lancer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk_Lancer::CState_Patrol_Walk_Lancer()
{
}

CState_Patrol_Walk_Lancer::~CState_Patrol_Walk_Lancer()
{
}

CState_Patrol_Walk_Lancer* CState_Patrol_Walk_Lancer::Create()
{
    CState_Patrol_Walk_Lancer* pInstance = new CState_Patrol_Walk_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Walk_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Walk_Lancer::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 13;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_WALK_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 13;

    m_fMyMaxLerp = 0.8f;
    m_fMyAccel = 100.f;

    m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 1.f;

    return __super::Initialize();
}

void CState_Patrol_Walk_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = rand() % 2 + 1;
    m_iDirectionRand = STATE_DIRECTION_N;

    m_fMaxSpeed = 1.f;
    m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

    m_vAIRandLook = -1.f;

    m_iWalkDelay = 20;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_iRand == 0)
            return AI_STATE_PATROL_DEFAULT_LANCER;
        else
            --m_iRand;
    }

    DoMove_AI_NoTarget(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Walk_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Walk_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Patrol_Walk_Lancer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
