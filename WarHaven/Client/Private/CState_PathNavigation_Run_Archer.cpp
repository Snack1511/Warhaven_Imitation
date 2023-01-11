#include "stdafx.h"
#include "CState_PathNavigation_Run_Archer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Run_Archer::CState_PathNavigation_Run_Archer()
{
}

CState_PathNavigation_Run_Archer::~CState_PathNavigation_Run_Archer()
{
}

HRESULT CState_PathNavigation_Run_Archer::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_PathNavigation_Run_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	

	pOwner->Set_AnimWeaponIndex(0, m_fInterPolationTime, m_fAnimSpeed);

	m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

	m_iStateChangeKeyFrame = 25;

	m_eSprintBeginState = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_ARCHER;

	m_fRand = frandom(0.2f, 0.5f);
	m_iRand = random(0, 5);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Run_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (m_fAIDelayTime > m_fRand)
	{
		switch (m_iRand)
		{
		case 0:
		case 1:
		case 2:

			return m_eSprintBeginState;


		case 3:

			return m_iAINextState;

		case 4:

			if(pAnimator->Is_CurAnimFinished())
				return m_eStateType;

			break;

		case 5:

			return m_eWalkState;

			break;

		default:
			return m_iAINextState;

		}

			
	}



    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Run_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_PathNavigation_Run_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

