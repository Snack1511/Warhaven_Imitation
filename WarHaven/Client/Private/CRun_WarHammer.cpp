#include "stdafx.h"
#include "CRun_WarHammer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_WarHammer::CRun_WarHammer()
{
}

CRun_WarHammer::~CRun_WarHammer()
{
}

HRESULT CRun_WarHammer::Initialize()
{

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
	m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);

	m_vecAdjState.push_back(STATE_CHANGE_PLAYER);


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.7f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.7f;
	



	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 1.5f;

	m_iStateChangeKeyFrame = 0;

    return S_OK;
}

void CRun_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;


	if (ePrevType == STATE_RUN_WARHAMMER_R || ePrevType == STATE_RUN_WARHAMMER_L)
	{
		m_fInterPolationTime = 0.f;
	}

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

		

	_uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.1f);

	if (iDirection < 8)
		m_iCurDirection = iDirection;

    return __super::Tick(pOwner, pAnimator);

}

void CRun_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run로 오는 조건
    1. 뛰어간다.
    */

	if (KEY(C, NONE))
	{
		// 걸어간다.
		if (KEY(W, HOLD) ||
			KEY(A, HOLD) ||
			KEY(D, HOLD))
		{
			if (KEY(S, NONE))
				return m_eStateType;
		}
		
	}


    return STATE_END;
}
