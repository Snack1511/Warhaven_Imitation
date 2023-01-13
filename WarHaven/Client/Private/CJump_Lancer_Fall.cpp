#include "stdafx.h"
#include "CJump_Lancer_Fall.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
CJump_Lancer_Fall::CJump_Lancer_Fall()
{
}

CJump_Lancer_Fall::~CJump_Lancer_Fall()
{
}


HRESULT CJump_Lancer_Fall::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 28;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = FLT_MAX;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = FLT_MIN;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_IDLE_ARCHER);


    m_vecAdjState.push_back(STATE_JUMP_LAND_LANCER);

    return S_OK;
}

CJump_Lancer_Fall* CJump_Lancer_Fall::Create()
{
    CJump_Lancer_Fall* pInstance = new CJump_Lancer_Fall();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Lancer_Fall");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CJump_Lancer_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    //if (ePrevType == STATE_JUMP_LANCER)
    //    m_fInterPolationTime = 0.05f;

    CUser::Get_Instance()->Clear_KeyCommands();
    m_pOwner = pOwner;
    m_fTimeAcc = 0.f;
    m_bExecuted = false;
    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fAnimSpeed);

   //__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Lancer_Fall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);
    pOwner->Set_DirAsLook();

    if (!pOwner->Is_Air())
        return STATE_IDLE_LANCER;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Lancer_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Lancer_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. Jump �� ������.
    */

    if(pAnimator->Is_CurAnimFinished())
        return m_eStateType;
  

    return STATE_END;
}
