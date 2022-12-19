#include "stdafx.h"
#include "CWalk_WarHammer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_WarHammer::CWalk_WarHammer()
{
}

CWalk_WarHammer::~CWalk_WarHammer()
{
}

HRESULT CWalk_WarHammer::Initialize()
{
    m_fInterPolationTime = 0.2f;


	//m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

    m_vecAdjState.push_back(STATE_CHANGE_PLAYER);
    m_vecAdjState.push_back(STATE_REVIVE_PLAYER);


	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;
	

    return S_OK;
}

void CWalk_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_BOUNCE_WARHAMMER_R)
        m_fInterPolationTime = 0.05f;

    /* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Move_Direction_Loop(pOwner, pAnimator, 0.05f);



    return __super::Tick(pOwner, pAnimator);
}

void CWalk_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 
    */


        // 천천히 
        if (KEY(C, HOLD))
        {
            // 걸어간다.
            if (KEY(W, HOLD) ||
                KEY(A, HOLD) ||
                KEY(D, HOLD))
            {

                return m_eStateType;
            }

        }

        if (KEY(S, HOLD))
            return m_eStateType;

        //CTRL 로 바꾸셈.
      
      

   


    return STATE_END;
}

