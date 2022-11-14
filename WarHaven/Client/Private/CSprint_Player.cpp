#include "stdafx.h"
#include "CSprint_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Player::CSprint_Player()
{
}

CSprint_Player::~CSprint_Player()
{
}

CSprint_Player* CSprint_Player::Create()
{
    CSprint_Player* pInstance = new CSprint_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Player::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 55;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
//    m_eStateType = STATE_SPRINT_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CSprint_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_eAnimType = ANIM_BASE_R;
    m_iAnimIndex = 55;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
        
    // ���� ����ƮŰ �� W �� �������� ��
    if (KEY(LSHIFT, NONE) || KEY(W, NONE))
    {
        // Sprint_End ȣ��
        if (m_iAnimIndex != 56)
        {
            m_iAnimIndex = 56;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
        }

        if (m_iAnimIndex == 56 && pAnimator->Is_CurAnimFinished())
        {

          //  return STATE_IDLE_PLAYER;
        }
    }

    // Sprint_End�� �������� ������ �� 
    if (m_iAnimIndex == 56 && pAnimator->Is_CurAnimFinished())
    {
        //return STATE_IDLE_PLAYER;
    }


    // Sprint Begin�� �������� ������ �� Sprint ȣ��
    if (m_iAnimIndex == 55 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 60;
     
        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    }


    // Sprint_Jump ������ ������ FallLoop ���� �������� �ڼ��� ����
    if (m_iAnimIndex == 58 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 57;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    }

    if (m_iAnimIndex == 57 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 56;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    }


    // Sprint ���¿��� Space �� ������ Sprint_Jump ���·� �Ѿ
    if (KEY(SPACE, TAP))
    {
        if (m_iAnimIndex != 58)
        {
            m_iAnimIndex = 58;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
        }
    }

    // ���� ��ư�� ���� �� (������Ʈ ���� �� )
    if (KEY(LBUTTON, TAP))
    {
        // SprintAttack_Begin
        if (m_iAnimIndex != 20)
        {
            m_iAnimIndex = 20;
            m_eAnimType = ANIM_ATTACK;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);

        }
    }

    // SprintAttack_Begin �� ���� �� 
    else if (m_iAnimIndex == 20 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 21;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    }

    else if (m_iAnimIndex == 21 && pAnimator->Is_CurAnimFinished())
    {
       // return STATE_IDLE_PLAYER;
    }

    if (m_iAnimIndex == 21 && pAnimator->Get_CurAnimFrame() > 20)
    {
        int a = 0;
    }


    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSprint_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����δ�.
    */


    if (KEY(LSHIFT, HOLD))
    {
        if(KEY(W, HOLD))
         return m_eStateType;

      
            return m_eStateType;
            

    }

    if (m_iAnimIndex == 56 ||
        m_iAnimIndex == 57 ||
        m_iAnimIndex == 58 || 
        m_iAnimIndex == 60)
        return m_eStateType;
   
    return STATE_END;
}
