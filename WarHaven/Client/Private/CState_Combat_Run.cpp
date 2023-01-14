#include "stdafx.h"
#include "CState_Combat_Run.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Run::CState_Combat_Run()
{
}

CState_Combat_Run::~CState_Combat_Run()
{
}

HRESULT CState_Combat_Run::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Combat_Run::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_ePreStateType = ePrevType;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Run::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Run::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    m_ePreStateType = STATE_END;

    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Run::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Run::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_EFFECTS, m_fAIDeafultVolume);
        break;

    case 1:
        Play_Sound(L"Env_FootStepGround", CHANNEL_EFFECTS, m_fAIDeafultVolume);
        break;
    }
}

