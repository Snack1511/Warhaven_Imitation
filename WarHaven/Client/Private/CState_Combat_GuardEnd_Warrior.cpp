#include "stdafx.h"
#include "CState_Combat_GuardEnd_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardEnd_Warrior::CState_Combat_GuardEnd_Warrior()
{
}

CState_Combat_GuardEnd_Warrior::~CState_Combat_GuardEnd_Warrior()
{
}

CState_Combat_GuardEnd_Warrior* CState_Combat_GuardEnd_Warrior::Create()
{
    CState_Combat_GuardEnd_Warrior* pInstance = new CState_Combat_GuardEnd_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardEnd_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardEnd_Warrior::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDEND_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

    m_iStateChangeKeyFrame = 10;

    return S_OK;
}

void CState_Combat_GuardEnd_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardEnd_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
        return AI_STATE_COMBAT_DEAFULT_WARRIOR_R;

	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CState_Combat_GuardEnd_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CState_Combat_GuardEnd_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

    if (pAnimator->Is_CurAnimFinished())
    {
        return m_eStateType;
    }
   
    return STATE_END;
}
