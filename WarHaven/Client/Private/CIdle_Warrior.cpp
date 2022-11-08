#include "stdafx.h"
#include "CIdle_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CIdle_Warrior::CIdle_Warrior()
{
}

CIdle_Warrior::~CIdle_Warrior()
{
}

CIdle_Warrior* CIdle_Warrior::Create()
{
    CIdle_Warrior* pInstance = new CIdle_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CIdle_Warrior::Initialize()
{
    // ����ü �и�.
   
    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 41;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
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

void CIdle_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CIdle_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CIdle_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CIdle_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
