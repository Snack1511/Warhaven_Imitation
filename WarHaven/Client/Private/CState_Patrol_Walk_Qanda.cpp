#include "stdafx.h"
#include "CState_Patrol_Walk_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk_Qanda::CState_Patrol_Walk_Qanda()
{
}

CState_Patrol_Walk_Qanda::~CState_Patrol_Walk_Qanda()
{
}

CState_Patrol_Walk_Qanda* CState_Patrol_Walk_Qanda::Create()
{
    CState_Patrol_Walk_Qanda* pInstance = new CState_Patrol_Walk_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Walk_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Walk_Qanda::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_WALK_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 22;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 23;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 24;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 25;

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

    Add_KeyFrame(32, 0, true);
    Add_KeyFrame(60, 0, true);
    return __super::Initialize();
}

void CState_Patrol_Walk_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = rand() % 2;
    m_iDirectionRand = rand() % 8;

    m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

    m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

    m_iWalkDelay = 20;

    m_eJumpFallStateType = AI_STATE_COMMON_LAND_QANDA;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_iRand == 0)
            return AI_STATE_PATROL_DEFAULT_QANDA;
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

void CState_Patrol_Walk_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Walk_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Patrol_Walk_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
