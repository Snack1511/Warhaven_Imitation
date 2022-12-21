#include "stdafx.h"
#include "CRun_Archer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Archer::CRun_Archer()
{
}

CRun_Archer::~CRun_Archer()
{
}

HRESULT CRun_Archer::Initialize()
{
	//m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;

	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);
	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_ARCHER);
	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_POISION_ARCHER);

	m_vecAdjState.push_back(STATE_GUARD_ARCHER);
	m_vecAdjState.push_back(STATE_ATTACK_SWING_ARCHER);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);

	m_vecAdjState.push_back(STATE_CHANGE_PLAYER);
	m_vecAdjState.push_back(STATE_REVIVE_PLAYER);
	m_vecAdjState.push_back(STATE_CANNON_PLAYER);


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    m_iStateChangeKeyFrame = 0;

	m_fInterPolationTime = 0.1f;
    return S_OK;
}

void CRun_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;


	if (ePrevType == STATE_RUN_ARCHER_R || ePrevType == STATE_RUN_ARCHER_L)
	{
		m_fInterPolationTime = 0.f;
	}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.1f);

	if (iDirection < 8)
		m_iCurDirection = iDirection;

    return __super::Tick(pOwner, pAnimator);

}

void CRun_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER가 Run로 오는 조건
    1. 뛰어간다.
    */

	if (KEY(C, NONE))
	{
		// 걸어간다.
		if (KEY(W, HOLD) ||
			KEY(A, HOLD) ||
			KEY(S, HOLD) ||
			KEY(D, HOLD))
		{

			return m_eStateType;
		}
		
	}


    return STATE_END;
}
