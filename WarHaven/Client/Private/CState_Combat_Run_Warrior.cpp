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
	__super::Initialize();

	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_Combat_Run_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = rand() % 2;
	m_iDirectionRand = rand() % 8;

	Set_Direction_Four_AI(m_iDirectionRand);
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iRunDelay = 25;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Run_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iFrame = pAnimator->Get_CurAnimFrame() + 1;

	if (m_iDirectionRand % 3 == 0)
	{
		if (iFrame == m_iRunDelay)
		{
			return m_eStateType;
		}
	}
	else if (m_iDirectionRand % 2 == 0)
	{
		if (iFrame == _uint(m_iRunDelay * 1.5f))
		{
			return m_eStateType;
		}
	}
	//else
	//	return AI_STATE_DEAFULT_IDLE_WARRIOR_R;

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
    return STATE_END;
}

