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
    m_vecAdjState.push_back(STATE_PLAYER_SKILL2);
    m_vecAdjState.push_back(STATE_PLAYER_SKILL1);

    m_iStateChangeKeyFrame = 9999;


    return S_OK;
}

void CRun_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */

    
    

    __super::Enter(pOwner, pAnimator, iPreAnimIndex);
}

STATE_TYPE CRun_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    for (int i = 0; i < STATE_DIRECTION_END; ++i)
    {
        if (m_VecDirectionAnimIndex[i] == m_iAnimIndex)
            m_iStateChangeKeyFrame = 0;
    }


    // Key(W)
    if (KEY(W, HOLD))
    {
        // Key(W + A)
        if (KEY(A, HOLD))
        {
            // RunBegin 
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NW])
            {
                if (m_iAnimIndex != m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NW])
                {
                    m_iAnimIndex = m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NW];

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);

                    m_iStateChangeKeyFrame = 9999;
                }

                // RunBegin  -> Run
                else
                {

                    if (m_iAnimIndex == m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NW] && pAnimator->Is_CurAnimFinished())
                    {
                        m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NW];

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                    }
                }
            }
        }

        // Key(W + D)
        else if (KEY(D, HOLD))
        {
            // RunBegin 
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NE])
            {
                if (m_iAnimIndex != m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NE])
                {
                    m_iAnimIndex = m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NE];

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);

                    m_iStateChangeKeyFrame = 9999;
                }

                // RunBegin  -> Run
                else
                {

                    if (m_iAnimIndex == m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NE] && pAnimator->Is_CurAnimFinished())
                    {
                        m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NE];

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                    }
                }

            }




        }

        // Key(W)
        else
        {
            // RunBegin 
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_N])
            {
                if (m_iAnimIndex != m_VecDirectionBeginAnimIndex[STATE_DIRECTION_N])
                {
                    m_iAnimIndex = m_VecDirectionBeginAnimIndex[STATE_DIRECTION_N];

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                    pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);

                    m_iStateChangeKeyFrame = 9999;

                }

                // RunBegin -> Run
                else
                {
                    if (m_iAnimIndex == m_VecDirectionBeginAnimIndex[STATE_DIRECTION_N] && pAnimator->Is_CurAnimFinished())
                    {
                        m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_N];

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                    }
                }
            }
        }
    }



    // Key(S)
    else if (KEY(S, HOLD))
    {
        // Key(S + A)
        if (KEY(A, HOLD))
        {
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SW])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SW];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }


        // Key(S + D)
        else if (KEY(D, HOLD))
        {
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SE])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SE];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
            }
        }



        // Key(S)
        else
        {
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_S])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_S];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
            }
        }

        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
    }

    // Key(A)
    else if (KEY(A, HOLD))
    {
        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_W])
        {
            // RunBegin 
            if (m_iAnimIndex != m_VecDirectionBeginAnimIndex[STATE_DIRECTION_W])
            {
                m_iAnimIndex = m_VecDirectionBeginAnimIndex[STATE_DIRECTION_W];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);

                m_iStateChangeKeyFrame = 9999;
            }

            // RunBegin -> Run
            else
            {
                if (m_iAnimIndex == m_VecDirectionBeginAnimIndex[STATE_DIRECTION_W] && pAnimator->Is_CurAnimFinished())
                {
                    m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_W];

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                }
            }
        }
    }

    // Key(D)
    else if (KEY(D, HOLD))
        {

        // RunBegin 
        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_E])
        {
            if (m_iAnimIndex != m_VecDirectionBeginAnimIndex[STATE_DIRECTION_E])
            {
                m_iAnimIndex = m_VecDirectionBeginAnimIndex[STATE_DIRECTION_E];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);

                m_iStateChangeKeyFrame = 9999;
            }

            // RunBegin -> Run
            else
            {
                if (m_iAnimIndex == m_VecDirectionBeginAnimIndex[STATE_DIRECTION_E] && pAnimator->Is_CurAnimFinished())
                {
                    m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_E];

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                }
            }
        }
        }

    //if (KEY(W, HOLD))
    //{
    //    // Key(CTRL + W + A)
    //    if (KEY(A, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NW])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NW];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //        }
    //    }

    //    // Key(CTRL + W + D)
    //    else if (KEY(D, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NE])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NE];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //        }
    //    }

    //    // Key(CTRL + W)
    //    else
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_N])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_N];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //        }
    //    }


    //}

    //// Key(CTRL + S)
    //else if (KEY(S, HOLD))
    //{

    //    // Key(CTRL + S + A)
    //    if (KEY(A, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SW])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SW];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //        }
    //    }

    //    // Key(CTRL + S + D)
    //    else if (KEY(D, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SE])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SE];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //        }
    //    }

    //    // Key(CTRL + S)
    //    else
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_S])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_S];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //        }
    //    }
    //}

    //// Key(CTRL + A)
    //else if (KEY(A, HOLD))
    //{
    //    // ����ó��
    //    if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_W])
    //    {
    //        m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_W];

    //        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //    }
    //}

    //// Key(CTRL + D)
    //else if (KEY(D, HOLD))
    //{
    //    // ����ó��
    //    if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_E])
    //    {
    //        m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_E];

    //        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //    }
    //}

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. 
    */

    // m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

    // W �� A �� ������ ���� ������ �̵��Ѵ�.
    // W �� D �� ������ ���� ������ �̵��Ѵ�.

    // ���� WASD �� �����ٸ�

    // ������ ���� �ʰ�
    if (KEY(SPACE, NONE))
    {
        // õõ�� 
        if (KEY(CTRL, HOLD))
        {
            // �ɾ��.
            if (KEY(W, HOLD) ||
                KEY(A, HOLD) ||
                KEY(S, HOLD) ||
                KEY(D, HOLD))
            {

                return m_eStateType;
            }

        }
    }
   
        //CTRL �� �ٲټ�.
      
      

   


    return STATE_END;
}
