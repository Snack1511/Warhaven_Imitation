#include "stdafx.h"
#include "CRun_Valkyrie.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Valkyrie::CRun_Valkyrie()
{
}

CRun_Valkyrie::~CRun_Valkyrie()
{
}

HRESULT CRun_Valkyrie::Initialize()
{

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);
	/*나중에 추가*/
	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

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

void CRun_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;


	if (ePrevType == STATE_RUN_VALKYRIE_R || ePrevType == STATE_RUN_VALKYRIE_L)
	{
		m_fInterPolationTime = 0.f;
	}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

		

	_uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.1f);

	if (iDirection < 8)
		m_iCurDirection = iDirection;

    return __super::Tick(pOwner, pAnimator);

}

void CRun_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Run로 오는 조건
		1. CTRL 을 누르지 않은 상태
		2. WAD 를 누른 상태
		*/

	if (KEY(CTRL, NONE))
	{
		// 걸어간다.
		if (KEY(W, HOLD) ||
			KEY(A, HOLD) ||
			KEY(D, HOLD))
		{

			return m_eStateType;
		}
		
	}


    return STATE_END;
}
