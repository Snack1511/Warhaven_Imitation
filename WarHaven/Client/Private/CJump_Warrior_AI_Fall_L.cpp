#include "stdafx.h"
#include "CJump_Warrior_AI_Fall_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Warrior_AI_Fall_L::CJump_Warrior_AI_Fall_L()
{
}

CJump_Warrior_AI_Fall_L::~CJump_Warrior_AI_Fall_L()
{
}

CJump_Warrior_AI_Fall_L* CJump_Warrior_AI_Fall_L::Create()
{
    CJump_Warrior_AI_Fall_L* pInstance = new CJump_Warrior_AI_Fall_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Warrior_AI_Fall_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_Warrior_AI_Fall_L::Initialize()
{



    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_FALL_WARRIOR_L_AI_ENEMY;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.15f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;


    //m_vecAdjState.push_back(STATE_JUMP_LAND_PLAYER_L);

    //m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


    return S_OK;
}

void CJump_Warrior_AI_Fall_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    //if (ePrevType == STATE_JUMP_PLAYER_L)
    //    m_fInterPolationTime = 0.05f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Warrior_AI_Fall_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return STATE_LAND_WARRIOR_L_AI_ENEMY;

   return __super::Tick(pOwner, pAnimator);
}

void CJump_Warrior_AI_Fall_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Warrior_AI_Fall_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. Jump �� ������.
    */

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
