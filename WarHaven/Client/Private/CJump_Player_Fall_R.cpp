#include "stdafx.h"
#include "CJump_Player_Fall_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
CJump_Player_Fall_R::CJump_Player_Fall_R()
{
}

CJump_Player_Fall_R::~CJump_Player_Fall_R()
{
}


HRESULT CJump_Player_Fall_R::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 10;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);

    m_vecAdjState.push_back(STATE_JUMP_LAND_PLAYER_R);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

CJump_Player_Fall_R* CJump_Player_Fall_R::Create()
{
    CJump_Player_Fall_R* pInstance = new CJump_Player_Fall_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Player_Fall_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CJump_Player_Fall_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == STATE_SPRINT_BEGIN_PLAYER ||
        ePrevType == STATE_SPRINT_LOOP_PLAYER ||
        ePrevType == STATE_SPRINT_END_PLAYER
        )
        m_fInterPolationTime = 0.2f;
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Player_Fall_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // ���� ����� �� 
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return STATE_JUMP_LAND_PLAYER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Player_Fall_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Player_Fall_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. Jump �� ������.
    */

    if(pAnimator->Is_CurAnimFinished())
        return m_eStateType;
  

    return STATE_END;
}
