#include "stdafx.h"
#include "CIdle_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CIdle_Player::CIdle_Player()
{
}

CIdle_Player::~CIdle_Player()
{
}

CIdle_Player* CIdle_Player::Create()
{
    CIdle_Player* pInstance = new CIdle_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Player::Initialize()
{
    // ����ü �и�.
    // 


    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 11;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_JUMP);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);


    return S_OK;
}

void CIdle_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner�� Animator Set Idle�� */

    // R_Idle L_Idle Setting
    if (m_eAnimType == ANIM_BASE_R)
    {
        m_iAnimIndex = 11;
    }
    else if (m_eAnimType == ANIM_BASE_L)
    {
        m_iAnimIndex = 3;
    }


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CIdle_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        // ���� R to RF ���¶��(������ -> ������ ����)
        if(m_iAnimIndex == 35)
        {
            // RF_Idle ���·� �ٲ۴�.
            m_iAnimIndex = 48;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }

        // ���� RF to L ���¶��(������ ���� -> ����)
        if (m_iAnimIndex == 53)
        {
            // RF_Idle ���·� �ٲ۴�.
            m_eAnimType = ANIM_BASE_L;
            m_iAnimIndex = 3;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }

        // ���� RF to LF ���¶��(������ ���� -> ���� ����)
        if (m_iAnimIndex == 54)
        {
            // LF_Idle ���·� �ٲ۴�.
            m_eAnimType = ANIM_BASE_L;
            m_iAnimIndex = 40;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }

        // ���� L to R ���¶��(���� -> ������)
        if (m_iAnimIndex == 27)
        {
            // R_Idle ���·� �ٲ۴�.
            m_eAnimType = ANIM_BASE_R;
            m_iAnimIndex = 11;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }

        // ���� LF to L ���¶��(���� ���� -> ������)
        if (m_iAnimIndex == 27)
        {
            // LF_Idle ���·� �ٲ۴�.
            m_eAnimType = ANIM_BASE_L;
            m_iAnimIndex = 3;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }

        if (m_iAnimIndex == 46)
        {
            // LF_Idle ���·� �ٲ۴�.
            m_eAnimType = ANIM_BASE_L;
            m_iAnimIndex = 11;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }

        

        //Switch_Right_And_Left(ANIM_BASE_R, 35, 48, pAnimator);
        //Switch_Right_And_Left(ANIM_BASE_R, 54, 40, pAnimator);
        //Switch_Right_And_Left(ANIM_BASE_R, 53, 3, pAnimator);
        //Switch_Right_And_Left(ANIM_BASE_L, 27, 11, pAnimator);
        //Switch_Right_And_Left(ANIM_BASE_L, 45, 3, pAnimator);
        //Switch_Right_And_Left(ANIM_BASE_L, 46, 11, pAnimator);
    }
        
    if (KEY(R, TAP))
    {
        // ���� ���������� ���������
        if (m_eAnimType == ANIM_BASE_R)
        {
            // Idle �����̸� && // ���� R to RF ���°� ������ �ʾҴٸ�
            if (m_iAnimIndex == 11 && m_iAnimIndex != 35)
            {
                m_iAnimIndex = 35;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // ���� �������� ���������
        else if (m_eAnimType == ANIM_BASE_L)
        {
            // Idle �����̸� && // ���� L to R ���°� ������ �ʾҴٸ�
            if (m_iAnimIndex == 3 && m_iAnimIndex != 27)
            {
                m_iAnimIndex = 27;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }
    }

    else if (KEY(E, TAP))
    {
        // ���� ������ �������� ���������
        if (m_eAnimType == ANIM_BASE_R)
        {
            // Idle �����̸� && RF to L ���°� ������ �ʾҴٸ�
            if (m_iAnimIndex == 48 && m_iAnimIndex != 53)
            {
                m_iAnimIndex = 53;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // ���� ���� �������� ���������
        else if (m_eAnimType == ANIM_BASE_L)
        {
            // Idle �����̸� && // ���� LF to L ���°� ������ �ʾҴٸ�
            if (m_iAnimIndex == 40 && m_iAnimIndex != 45)
            {
                m_iAnimIndex = 45;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

       
    }

    else  if (KEY(Q, TAP))
    {
        // ���� ������ �������� ���������
        if (m_eAnimType == ANIM_BASE_R)
        {
            // Idle �����̸� && RF to LF ���°� ������ �ʾҴٸ�
            if (m_iAnimIndex == 48 && m_iAnimIndex != 54)
            {
                m_iAnimIndex = 54;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // ���� ���� �������� ���������
        else if (m_eAnimType == ANIM_BASE_L)
        {
            // Idle �����̸� && // ���� LF to R ���°� ������ �ʾҴٸ�
            if (m_iAnimIndex == 40 && m_iAnimIndex != 46)
            {
                m_iAnimIndex = 46;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }
    }

    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ���� 
    1. ���� �������� �ִϸ��̼��� ������ ��
    */



    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}

void CIdle_Player::Switch_Right_And_Left(_uint iAnimType, _uint iAnimIndex, _uint iChangeAnimIndex, CAnimator* pAnimator)
{
    if (m_eAnimType == iAnimType == m_iAnimIndex == iAnimIndex)
    {
        m_iAnimIndex = iChangeAnimIndex;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    }
}
