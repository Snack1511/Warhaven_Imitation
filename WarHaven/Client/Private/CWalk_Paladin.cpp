#include "stdafx.h"
#include "CWalk_Paladin.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Paladin::CWalk_Paladin()
{
}

CWalk_Paladin::~CWalk_Paladin()
{
}

HRESULT CWalk_Paladin::Initialize()
{
    m_fInterPolationTime = 0.2f;

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;


	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;
	

    return S_OK;
}

void CWalk_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Move_Direction_Loop(pOwner, pAnimator, 0.05f);



    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(C, HOLD))
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

