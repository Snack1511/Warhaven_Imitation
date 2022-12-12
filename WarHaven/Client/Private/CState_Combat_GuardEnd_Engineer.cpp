#include "stdafx.h"
#include "CState_Combat_GuardEnd_Engineer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardEnd_Engineer::CState_Combat_GuardEnd_Engineer()
{
}

CState_Combat_GuardEnd_Engineer::~CState_Combat_GuardEnd_Engineer()
{
}

CState_Combat_GuardEnd_Engineer* CState_Combat_GuardEnd_Engineer::Create()
{
    CState_Combat_GuardEnd_Engineer* pInstance = new CState_Combat_GuardEnd_Engineer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardEnd_Engineer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardEnd_Engineer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDEND_ENGINEER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

    m_iStateChangeKeyFrame = 10;

    return S_OK;
}

void CState_Combat_GuardEnd_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardEnd_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
        return AI_STATE_COMBAT_DEAFULT_ENGINEER_R;

	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CState_Combat_GuardEnd_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CState_Combat_GuardEnd_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
