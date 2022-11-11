#include "stdafx.h"
#include "CWalk_Player_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Player_L::CWalk_Player_L()
{
}

CWalk_Player_L::~CWalk_Player_L()
{
}

CWalk_Player_L* CWalk_Player_L::Create()
{
    CWalk_Player_L* pInstance = new CWalk_Player_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Player_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Player_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
//    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    m_VecDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
    m_VecDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
    m_VecDirectionAnimIndex[STATE_DIRECTION_N] = 31;
    m_VecDirectionAnimIndex[STATE_DIRECTION_SW] = 36;

    m_VecDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_VecDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_VecDirectionAnimIndex[STATE_DIRECTION_W] = 37;
    m_VecDirectionAnimIndex[STATE_DIRECTION_E] = 30;

    return S_OK;
}

void CWalk_Player_L::Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex)
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, iPreAnimIndex);
}

STATE_TYPE CWalk_Player_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_PLAYER_L;
    }

    //if (
    //    KEY(W, NONE) &&
    //    KEY(A, NONE) &&
    //    KEY(S, NONE) &&
    //    KEY(D, NONE)
    //    )
    //{
    //    return STATE_IDLE_PLAYER_L;
    //}  
    //else
    //{

    //    if (KEY(W, HOLD))
    //    {
    //        // Key(CTRL + W + A)
    //        if (KEY(A, HOLD))
    //        {
    //            //Base_L
    //            if (m_eAnimType == ANIM_BASE_L)
    //            {
    //                // ����ó��
    //                if (m_iAnimIndex != m_VecDirectionAnimIndex[0])
    //                {
    //                    m_iAnimIndex = m_VecDirectionAnimIndex[0];

    //                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //                }
    //            }

    //        }

    //        // Key(CTRL + W + D)
    //        else if (KEY(D, HOLD))
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != m_VecDirectionAnimIndex[1])
    //            {
    //                m_iAnimIndex = m_VecDirectionAnimIndex[1];

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + W)
    //        else
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != m_VecDirectionAnimIndex[2])
    //            {
    //                m_iAnimIndex = m_VecDirectionAnimIndex[2];

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //            }
    //        }


    //    }

    //    // Key(CTRL + S)
    //    else if (KEY(S, HOLD))
    //    {
    //        // Key(CTRL + S + A)
    //        if (KEY(A, HOLD))
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != m_VecDirectionAnimIndex[3])
    //            {
    //                m_iAnimIndex = m_VecDirectionAnimIndex[3];

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + S + D)
    //        else if (KEY(D, HOLD))
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != m_VecDirectionAnimIndex[4])
    //            {
    //                m_iAnimIndex = m_VecDirectionAnimIndex[4];

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + S)
    //        else
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != m_VecDirectionAnimIndex[5])
    //            {
    //                m_iAnimIndex = m_VecDirectionAnimIndex[5];

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }
    //    }

    //    // Key(CTRL + A)
    //    else if (KEY(A, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[6])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[6];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //        }
    //    }

    //    // Key(CTRL + D)
    //    else if (KEY(D, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_VecDirectionAnimIndex[7])
    //        {
    //            m_iAnimIndex = m_VecDirectionAnimIndex[7];

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //        }
    //    }
    //}

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Player_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Player_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
