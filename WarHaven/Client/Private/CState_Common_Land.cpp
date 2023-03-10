#include "stdafx.h"
#include "CState_Common_Land.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"
#include "CBehavior.h"

#include "CUser.h"

CState_Common_Land::CState_Common_Land()
{
}

CState_Common_Land::~CState_Common_Land()
{
}

HRESULT CState_Common_Land::Initialize()
{
    __super::Initialize();

    //m_iStateChangeKeyFrame = 20;

    //// 선형 보간 시간
    //m_fInterPolationTime = 0.05f;

    //// 애니메이션의 전체 속도를 올려준다.
    //m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_Common_Land::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Land::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
    {
        eBehaviorType  BehavType = pOwner->Get_OwnerPlayer()->Get_CurBehavior()->Get_BehaviorType();
        STATE_TYPE eNewState = STATE_END;

        switch (BehavType)
        {
        case Client::eBehaviorType::ePatrol:
        case Client::eBehaviorType::eChange:
        case Client::eBehaviorType::eRevive:
        case Client::eBehaviorType::ePadenCannonInteract:
        default:
            
            eNewState = pOwner->Get_AIState_Type().eAIPatrolDefaultState;
            
            break;

        case Client::eBehaviorType::ePathFinding:
        case Client::eBehaviorType::eGliding:

            eNewState = pOwner->Get_AIState_Type().eAIPathFindDefaultState;

            break;

        case Client::eBehaviorType::eCombat:
        case Client::eBehaviorType::eCatchCannon:

            eNewState = pOwner->Get_AIState_Type().eAICommbatDefaultState;

            break;

        }

        return eNewState;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Land::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Land::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Common_Land::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_EFFECTS);
        break;
    }
}

