#include "stdafx.h"
#include "CWalk_Valkyrie.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Valkyrie::CWalk_Valkyrie()
{
}

CWalk_Valkyrie::~CWalk_Valkyrie()
{
}

HRESULT CWalk_Valkyrie::Initialize()
{
    m_fInterPolationTime = 0.2f;


	////m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);
 //   m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
 //   m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
 //   m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
 //   m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


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

void CWalk_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Move_Direction_Loop(pOwner, pAnimator, 0.05f);



    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 
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
        //CTRL 로 바꾸셈.
      
      

   


    return STATE_END;
}

