#include "stdafx.h"
#include "CJump_Priest_Fall.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
CJump_Priest_Fall::CJump_Priest_Fall()
{
}

CJump_Priest_Fall::~CJump_Priest_Fall()
{
}


HRESULT CJump_Priest_Fall::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_IDLE_ARCHER);

    m_vecAdjState.push_back(STATE_JUMP_LAND_PRIEST);
    m_vecAdjState.push_back(STATE_AIRDASH_PRIEST);
    
    Init_AttackState_Priest();
    Init_CommonState_Player();



    return S_OK;
}

CJump_Priest_Fall* CJump_Priest_Fall::Create()
{
    CJump_Priest_Fall* pInstance = new CJump_Priest_Fall();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Priest_Fall");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CJump_Priest_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == STATE_JUMP_PRIEST)
        m_fInterPolationTime = 0.05f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Priest_Fall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // ���� ����� �� 
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return STATE_JUMP_LAND_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Priest_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Priest_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. Jump �� ������.
    */

    if(pAnimator->Is_CurAnimFinished())
        return m_eStateType;
  

    return STATE_END;
}
