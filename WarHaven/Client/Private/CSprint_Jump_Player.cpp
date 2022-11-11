#include "stdafx.h"
#include "CSprint_Jump_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Jump_Player::CSprint_Jump_Player()
{
}

CSprint_Jump_Player::~CSprint_Jump_Player()
{
}

CSprint_Jump_Player* CSprint_Jump_Player::Create()
{
    CSprint_Jump_Player* pInstance = new CSprint_Jump_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Jump_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Jump_Player::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 58;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_JUMP_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

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
    m_vecAdjState.push_back(STATE_SPRINT_JUMPFALL_PLAYER);


    return S_OK;
}

void CSprint_Jump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{

    __super::Enter(pOwner, pAnimator, ePrevType);

}

STATE_TYPE CSprint_Jump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Jump_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSprint_Jump_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����Ѵ�.
    */


    if (KEY(SPACE, TAP))
    {
        return m_eStateType;
    }


   
    return STATE_END;
}
