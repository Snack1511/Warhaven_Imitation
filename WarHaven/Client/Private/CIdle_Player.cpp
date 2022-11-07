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
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
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
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CIdle_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner�� Animator Set Idle�� */

  


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CIdle_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
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
