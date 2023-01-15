#include "stdafx.h"
#include "CState_Patrol_Walk_Priest.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk_Priest::CState_Patrol_Walk_Priest()
{
}

CState_Patrol_Walk_Priest::~CState_Patrol_Walk_Priest()
{
}

CState_Patrol_Walk_Priest* CState_Patrol_Walk_Priest::Create()
{
    CState_Patrol_Walk_Priest* pInstance = new CState_Patrol_Walk_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Walk_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Walk_Priest::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 39;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_WALK_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;

    m_fMyMaxLerp = 1.2f;
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

    m_fAnimSpeed = 2.5f;

    return __super::Initialize();
}

void CState_Patrol_Walk_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = rand() % 2;
    m_iDirectionRand = rand() % 8;

    m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

    m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

    m_iWalkDelay = 20;

    m_eJumpFallStateType = AI_STATE_COMMON_LAND_PRIEST;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CUnit* pCurObject = static_cast<CUnit*>(pOwner->Get_CureObject());

    if (pCurObject)
    {
        if (pCurObject->Get_Status().fHP < pCurObject->Get_Status().fMaxHP)
            return AI_STATE_PATROL_CURE_BEGIN_PRIEST;
    }

    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_iRand == 0)
            return AI_STATE_PATROL_DEFAULT_PRIEST;
        else
            --m_iRand;
    }


    _uint iFrame = pAnimator->Get_CurAnimFrame() + 1;

    if (m_iDirectionRand % 3 == 0)
    {
        if (iFrame == m_iWalkDelay)
        {
            return m_eStateType;
        }
    }
    else if(m_iDirectionRand % 2 == 0)
    {
        if (iFrame == m_iWalkDelay * 2)
        {
            return m_eStateType;
        }
    }


    DoMove_AI_NoTarget(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Walk_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Walk_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Patrol_Walk_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
