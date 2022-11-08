#include "stdafx.h"
#include "CWalk_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Player::CWalk_Player()
{
}

CWalk_Player::~CWalk_Player()
{
}

CWalk_Player* CWalk_Player::Create()
{
    CWalk_Player* pInstance = new CWalk_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Player::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CWalk_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CWalk_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
        return STATE_IDLE_PLAYER;


    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. 
    */

    // ���� WASD �� �����ٸ�
    if (KEY(W, HOLD) ||
        KEY(A, HOLD) ||
        KEY(S, HOLD) ||
        KEY(D, HOLD))
    {
     
        // ���� CTRL �� �����ٸ�
        if (KEY(Z, HOLD))
        {
            // ���� W �� �����ٸ�(����)
            if (KEY(W, HOLD))
            {
                // A �� �����ٸ�(�������)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 33;
                }

                // A �� �����ٸ�(���������)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 44;
                }
            }

            // ���� S �� �����ٸ�(�Ĺ�)
            else if (KEY(S, HOLD))
            {
                // ���� A �� �����ٸ�(�Ĺ����)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 33;
                }

                // ���� A �� �����ٸ�(�Ĺ������)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 44;
                }
            }

            
            // A�� �����ٸ�(����)
            else if(KEY(A, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }

            // D�� �����ٸ�(������)
            else if (KEY(D, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }
        }
        else
        {
            // ���� W �� �����ٸ�(����)
            if (KEY(W, HOLD))
            {
                // A �� �����ٸ�(�������)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 22;
                }

                // A �� �����ٸ�(���������)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 26;
                }
            }

            // ���� S �� �����ٸ�(�Ĺ�)
            else if (KEY(S, HOLD))
            {
                // ���� A �� �����ٸ�(�Ĺ����)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 33;
                }

                // ���� A �� �����ٸ�(�Ĺ������)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 44;
                }
            }


            // A�� �����ٸ�(����)
            else if (KEY(A, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }

            // D�� �����ٸ�(������)
            else if (KEY(D, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }
        }

        return m_eStateType;
    }




    return STATE_END;
}
