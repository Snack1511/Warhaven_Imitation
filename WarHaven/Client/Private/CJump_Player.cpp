#include "stdafx.h"
#include "CJump_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Player::CJump_Player()
{
}

CJump_Player::~CJump_Player()
{
}

CJump_Player* CJump_Player::Create()
{
    CJump_Player* pInstance = new CJump_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Player::Initialize()
{



    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_IDLE_PLAYER);


    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CJump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner�� Animator Set Idle�� */

    if (m_eAnimType == ANIM_BASE_R)
    {
        if (KEY(W, HOLD))
            m_iAnimIndex = 14;

        else if (KEY(S, HOLD))
            m_iAnimIndex = 15;

        else if (KEY(A, HOLD))
            m_iAnimIndex = 16;

        else if (KEY(D, HOLD))
            m_iAnimIndex = 13;

        else
            m_iAnimIndex = 12;
    }
  

    if (m_eAnimType == ANIM_BASE_L)
    {
        if (KEY(W, HOLD))
            m_iAnimIndex = 6;

        else if (KEY(S, HOLD))
            m_iAnimIndex = 7;

        else if (KEY(A, HOLD))
            m_iAnimIndex = 8;

        else if (KEY(D, HOLD))
            m_iAnimIndex = 5;

        else
            m_iAnimIndex = 4;
    }


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CJump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    // �������� ������ ��
    if (pAnimator->Is_CurAnimFinished())
    {
        // Base_R
        if (m_eAnimType == ANIM_BASE_R)
        {
            // ���� ���� ���°� ������ ���� ���� ��
            if (m_iAnimIndex != 10 && m_iAnimIndex != 17)
            {
                m_iAnimIndex = 10;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            // ���� �߿��� ���� �� ��
            else if (m_iAnimIndex == 10)
            {
                m_iAnimIndex = 17;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            //������ ���� ��
            else
            {
                return STATE_IDLE_PLAYER;
            }
        }

        // Base_L
        if (m_eAnimType == ANIM_BASE_L)
        {
            // ���� ���� ���°� ������ ���� ���� ��
            if (m_iAnimIndex != 1 && m_iAnimIndex != 9)
            {
                m_iAnimIndex = 1;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            // ���� �߿��� ���� �� ��
            else if (m_iAnimIndex == 1)
            {
                m_iAnimIndex = 9;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            //������ ���� ��
            else
            {
                return STATE_IDLE_PLAYER;
            }
        }
        
    }
        
   
    return __super::Tick(pOwner, pAnimator);


    
}

void CJump_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. 
    */

    // m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

    // W �� A �� ������ ���� ������ �̵��Ѵ�.
    // W �� D �� ������ ���� ������ �̵��Ѵ�.

    // ���� WASD �� �����ٸ�
    if (KEY(SPACE, TAP))
    {
        return m_eStateType;
    }
   
        //CTRL �� �ٲټ�.
      
      

   


    return STATE_END;
}
