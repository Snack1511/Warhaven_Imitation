#include "stdafx.h"
#include "CRun_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Player::CRun_Player()
{
}

CRun_Player::~CRun_Player()
{
}

HRESULT CRun_Player::Initialize()
{
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

    m_vecAdjState.push_back(STATE_SLIDE_BEGIN_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);


    m_iStateChangeKeyFrame = 0;


    return S_OK;
}

void CRun_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner의 Animator Set Idle로 */

    
    if (ePrevType == STATE_RUN_PLAYER_R)
    {
        pAnimator->Set_CurFrame(22);
    }

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CRun_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(W, HOLD))
    {
        // Key(CTRL + W + A)
        if (KEY(A, HOLD))
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NW])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NW];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + W + D)
        else if (KEY(D, HOLD))
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NE])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NE];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + W)
        else
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_N])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_N];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
            }
        }


    }

    // Key(CTRL + S)
    else if (KEY(S, HOLD))
    {

        // Key(CTRL + S + A)
        if (KEY(A, HOLD))
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SW])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SW];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + S + D)
        else if (KEY(D, HOLD))
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SE])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SE];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + S)
        else
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_S])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_S];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }
    }

    // Key(CTRL + A)
    else if (KEY(A, HOLD))
    {
        // 예외처리
        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_W])
        {
            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_W];

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }
    }

    // Key(CTRL + D)
    else if (KEY(D, HOLD))
    {
        // 예외처리
        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_E])
        {
            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_E];

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
        }
    }

    return __super::Tick(pOwner, pAnimator);

}

void CRun_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 
    */

    if (KEY(SPACE, NONE))
    {
        // 천천히 
        if (KEY(CTRL, NONE))
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
    }
   
        //CTRL 로 바꾸셈.
      
      

   


    return STATE_END;
}
