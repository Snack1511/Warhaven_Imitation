#include "stdafx.h"
#include "CWalk_SpearMan.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_SpearMan::CWalk_SpearMan()
{
}

CWalk_SpearMan::~CWalk_SpearMan()
{
}

HRESULT CWalk_SpearMan::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	/*Attack*/
	m_vecAdjState.push_back(STATE_SPEARMAN_GUARDBREAK);
	m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN);

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;
	

    return S_OK;
}

void CWalk_SpearMan::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

	Physics_Setting(pOwner->Get_Status().fRunSpeed, pOwner, true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_SpearMan::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	//if (KEY(RBUTTON, TAP))
	//{
	//	pAnimator->Set_CurAnimIndex(ANIM_ATTACK, 0, ANIM_DIVIDE::eBODYUPPER);
	//}

	Move_Direction_Loop(pOwner, pAnimator, 0.05f);

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PLAYER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_SpearMan::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_SpearMan::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. CTRL + WASD 
    */

    // 천천히 
    if (KEY(CTRL, HOLD))
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

