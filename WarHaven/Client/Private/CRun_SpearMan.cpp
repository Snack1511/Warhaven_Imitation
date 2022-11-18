#include "stdafx.h"
#include "CRun_SpearMan.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_SpearMan::CRun_SpearMan()
{
}

CRun_SpearMan::~CRun_SpearMan()
{
}

HRESULT CRun_SpearMan::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;

	m_vecAdjState.push_back(STATE_SPEARMAN_GUARDBREAK);
	m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN);


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.2f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.2f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.2f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.2f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.2f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.2f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 2.2f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 2.2f;

    m_iStateChangeKeyFrame = 0;

	m_fInterPolationTime = 0.1f;
	m_fMaxSpeed = 4.f;
    return S_OK;
}

void CRun_SpearMan::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;

	Physics_Setting(pOwner->Get_Status().fRunSpeed, pOwner, true);

    
	if (ePrevType == STATE_RUN_SPEARMAN_R || ePrevType == STATE_RUN_SPEARMAN_L)
	{
		m_fInterPolationTime = 0.f;
		pAnimator->Set_CurFrame(22);
	}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_SpearMan::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Move_Direction_Loop(pOwner, pAnimator, 0.1f);

    return __super::Tick(pOwner, pAnimator);

}

void CRun_SpearMan::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_SpearMan::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* SPEARMAN가 Walk로 오는 조건
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
