#include "stdafx.h"
#include "CWalk_Player_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Player_R::CWalk_Player_R()
{
}

CWalk_Player_R::~CWalk_Player_R()
{
}

CWalk_Player_R* CWalk_Player_R::Create()
{
    CWalk_Player_R* pInstance = new CWalk_Player_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Player_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Player_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.

    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);
//    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    m_VecDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_VecDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_VecDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_VecDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
                            
    m_VecDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_VecDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_VecDirectionAnimIndex[STATE_DIRECTION_W] = 45;
    m_VecDirectionAnimIndex[STATE_DIRECTION_E] = 38;



    return S_OK;
}

void CWalk_Player_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWalk_Player_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_PLAYER_R;
    }

    //if (
    //    KEY(W, NONE) &&
    //    KEY(A, NONE) &&
    //    KEY(S, NONE) &&
    //    KEY(D, NONE)
    //    )
    //{
    //    return STATE_IDLE_PLAYER_R;
    //}

    //else
    //{

    //    if (KEY(W, HOLD))
    //    {
    //        // Key(CTRL + W + A)
    //        if (KEY(A, HOLD))
    //        {
    //            // ����ó��r
    //            if (m_iAnimIndex != 41)
    //            {
    //                m_iAnimIndex = 41;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + W + D)
    //        else if (KEY(D, HOLD))
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != 40)
    //            {
    //                    
    //                m_iAnimIndex = 40;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + W)
    //        else
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != 39)
    //            {
    //                m_iAnimIndex = 39;

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
    //            if (m_iAnimIndex != 44)
    //            {
    //                m_iAnimIndex = 44;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + S + D)
    //        else if (KEY(D, HOLD))
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != 43)
    //            {
    //                m_iAnimIndex = 43;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + S)
    //        else
    //        {
    //            // ����ó��
    //            if (m_iAnimIndex != 42)
    //            {
    //                m_iAnimIndex = 42;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //            }
    //        }
    //    }

    //    // Key(CTRL + A)
    //    else if (KEY(A, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != 45)
    //        {
    //            m_iAnimIndex = 45;

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //        }
    //    }

    //    // Key(CTRL + D)
    //    else if (KEY(D, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != 38)
    //        {
    //            m_iAnimIndex = 38;

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //        }
    //    }
    //}

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Player_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Player_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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

   


    return STATE_END;
}
