#include "stdafx.h"
#include "CState_Common_Land_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Land_Engineer::CState_Common_Land_Engineer()
{
}

CState_Common_Land_Engineer::~CState_Common_Land_Engineer()
{
}

HRESULT CState_Common_Land_Engineer::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_Common_Land_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    //switch (ePrevType)
    //{
    //case Client::STATE_SPRINT_JUMP_PLAYER:
    //case Client::STATE_SPRINT_JUMPFALL_PLAYER:
    //    pOwner->Enter_State(STATE_SPRINT_END_PLAYER);
    //    return;

    //    break;
    //default:
    //    break;
    //}

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Land_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Land_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Land_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

