#include "stdafx.h"
#include "CState_Patrol_Walk_Fiona.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk_Fiona::CState_Patrol_Walk_Fiona()
{
}

CState_Patrol_Walk_Fiona::~CState_Patrol_Walk_Fiona()
{
}

HRESULT CState_Patrol_Walk_Fiona::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;
	

	m_iStateChangeKeyFrame = 0;

	m_fAnimSpeed = 2.5f;

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 29;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 30;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 31;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 32;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 33;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 34;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 35;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 36;

    return S_OK;
}

void CState_Patrol_Walk_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = rand() % 2;
	m_iDirectionRand = rand() % 8;

	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iWalkDelay = 20;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
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

void CState_Patrol_Walk_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Patrol_Walk_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Patrol_Walk_Fiona::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

