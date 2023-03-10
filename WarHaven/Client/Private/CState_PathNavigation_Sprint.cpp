#include "stdafx.h"
#include "CState_PathNavigation_Sprint.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint::CState_PathNavigation_Sprint()
{
}

CState_PathNavigation_Sprint::~CState_PathNavigation_Sprint()
{
}

HRESULT CState_PathNavigation_Sprint::Initialize()
{
    return __super::Initialize();
}

void CState_PathNavigation_Sprint::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Is_Air())
        return m_eJumpFallStateType;

    return STATE_END;
}

void CState_PathNavigation_Sprint::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.3f);
        break;


    case 222:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.3f);
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    case 333:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.3f);
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    default:
        break;
    }
}


