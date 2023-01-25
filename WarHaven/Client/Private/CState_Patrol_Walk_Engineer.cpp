#include "stdafx.h"
#include "CState_Patrol_Walk_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk_Engineer::CState_Patrol_Walk_Engineer()
{
}

CState_Patrol_Walk_Engineer::~CState_Patrol_Walk_Engineer()
{
}

HRESULT CState_Patrol_Walk_Engineer::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 1.f;
	

	Add_KeyFrame(14, 0, true);
	Add_KeyFrame(59, 0, true);

    return S_OK;
}

void CState_Patrol_Walk_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = rand() % 2;
	m_iDirectionRand = rand() % 8;

	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iWalkDelay = 20;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

void CState_Patrol_Walk_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Patrol_Walk_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Patrol_Walk_Engineer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

