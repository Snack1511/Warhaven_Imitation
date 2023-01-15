#include "stdafx.h"
#include "CState_PathNavigation_Run.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Run::CState_PathNavigation_Run()
{
}

CState_PathNavigation_Run::~CState_PathNavigation_Run()
{
}

HRESULT CState_PathNavigation_Run::Initialize()
{
    return __super::Initialize();
}

void CState_PathNavigation_Run::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fRand = frandom(0.2f, 0.5f);
	m_iRand = random(RUN_STATE_LOOP1, RUN_STATE_WALK);

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Run::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (m_fAIDelayTime > m_fRand)
	{
		switch (m_iRand)
		{
		case RUN_STATE_LOOP1:
		case RUN_STATE_LOOP2:
		case RUN_STATE_LOOP3:

			return m_eSprintBeginState;


		case RUN_STATE_JUMP:

			return m_eJumpState;

		case RUN_STATE_STOP:

			if (pAnimator->Is_CurAnimFinished())
				return m_eStateType;

			break;

		case RUN_STATE_WALK:

			return m_eWalkState;

			break;

		default:
			return m_eSprintBeginState;

		}

	}

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Run::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Run::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_PathNavigation_Run::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepGround", CHANNEL_EFFECTS, m_fAIDeafultVolume);
		break;
	}
}

