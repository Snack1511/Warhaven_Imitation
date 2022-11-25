#include "stdafx.h"
#include "CWalk_Warrior_Master.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Warrior_Master::CWalk_Warrior_Master()
{
}

CWalk_Warrior_Master::~CWalk_Warrior_Master()
{
}

HRESULT CWalk_Warrior_Master::Initialize()
{
    m_fInterPolationTime = 0.2f;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;
	
    return S_OK;
}

void CWalk_Warrior_Master::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Warrior_Master::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Warrior_Master::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Warrior_Master::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
   // return __super::Check_Condition(pOwner, pAnimator);
}

