#include "stdafx.h"
#include "CState_PathNavigation_Walk_Warrior.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Walk_Warrior::CState_PathNavigation_Walk_Warrior()
{
}

CState_PathNavigation_Walk_Warrior::~CState_PathNavigation_Walk_Warrior()
{
}

HRESULT CState_PathNavigation_Walk_Warrior::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 0;

	m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Walk_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));
	m_iRand = random(0, 2);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_iRand != 0)
		return m_eStateType;
	else
		return m_iAINextState;

	DoMove_AI_NoTarget(pOwner, pAnimator);

    return CState::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_PathNavigation_Walk_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

