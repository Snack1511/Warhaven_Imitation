#include "stdafx.h"
#include "CState_Patrol_Walk.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk::CState_Patrol_Walk()
{
}

CState_Patrol_Walk::~CState_Patrol_Walk()
{
}

HRESULT CState_Patrol_Walk::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Patrol_Walk::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Walk::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Walk::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Patrol_Walk::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, m_fAIDeafultVolume);
        break;
    }
}

