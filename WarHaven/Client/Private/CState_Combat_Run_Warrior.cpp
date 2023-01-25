#include "stdafx.h"
#include "CState_Combat_Run_Warrior.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Run_Warrior::CState_Combat_Run_Warrior()
{
}

CState_Combat_Run_Warrior::~CState_Combat_Run_Warrior()
{
}

HRESULT CState_Combat_Run_Warrior::Initialize()
{
	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;
	
	Add_KeyFrame(2, 0, true);
	Add_KeyFrame(27, 0, true);

    return __super::Initialize();
}

void CState_Combat_Run_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = rand() % 8;
	m_iDirectionRand = rand() % 8;

	Set_Direction_Front_AI(m_iDirectionRand);
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iStateChangeKeyFrame = 25;

	m_fAIMyLength = 2.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Run_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iFrame = pAnimator->Get_CurAnimFrame() + 1;

	switch (m_iDirectionRand)
	{
	case 0:
	case 1:
	case 2:

		if (iFrame == m_iStateChangeKeyFrame)
			return m_eStateType;

		break;

	case 3:
	case 4:
	case 5:
	case 6:

		if (iFrame == _uint(_float(m_iStateChangeKeyFrame) * 1.5f))
			return m_eStateType;

		break;

	case 7:

		if (pOwner->Can_Use(CUnit::SKILL2))
			return AI_STATE_COMBAT_OXEN_LOOPATTACK_WARRIOR;

		else if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
			return m_eStateType;

		break;

	default:
		break;
	}


	DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Run_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Combat_Run_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Run_Warrior::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
