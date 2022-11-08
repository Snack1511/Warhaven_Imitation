#include "stdafx.h"
#include "CIdle_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CIdle_Warrior.h"

#include "CState_Manager.h"

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
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    //m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    //m_iAnimIndex = 41;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    //m_eStateType = STATE_IDLE_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
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
    //m_vecAdjState.push_back(STATE_WALK);


    // m_vecAdjState.push_back(STATE_END);

    return S_OK;
}

void CIdle_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner�� Animator Set Idle�� */


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CIdle_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Test_Animation_Loop(pOwner, pAnimator);

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

void CIdle_Player::Test_Animation_Loop(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(LEFT, TAP))
    {
        if(0 < m_iAnimIndex)
            --m_iAnimIndex;
        Enter(pOwner, pAnimator);
    }

    if (KEY(RIGHT, TAP))
    {
        ++m_iAnimIndex;
        Enter(pOwner, pAnimator);
    }

    if (KEY(UP, TAP))
    {
        if (m_eAnimType == ANIM_END)
            return;

        _uint iType = m_eAnimType;

        ++iType;

        m_eAnimType = (ANIM_TYPE)iType;

        m_iAnimIndex = 0;

        Enter(pOwner, pAnimator);
    }

    if (KEY(DOWN, TAP))
    {
        if (m_eAnimType == ANIM_BASE_R)
            return;

        _uint iType = m_eAnimType;

        --iType;

        m_eAnimType = (ANIM_TYPE)iType;

        m_iAnimIndex = 0;

        Enter(pOwner, pAnimator);
    }

    if (KEY(Q, TAP))
    {
        _uint iType = m_eStateType;
        
        --iType;

        m_eStateType = (STATE_TYPE)iType;
    }

    if (KEY(E, TAP))
    {
        _uint iType = m_eStateType;

        ++iType;

        m_eStateType = (STATE_TYPE)iType;
    }

}
