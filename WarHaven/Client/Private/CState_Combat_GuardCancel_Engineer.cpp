#include "stdafx.h"
#include "CState_Combat_GuardCancel_Engineer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardCancel_Engineer::CState_Combat_GuardCancel_Engineer()
{
}

CState_Combat_GuardCancel_Engineer::~CState_Combat_GuardCancel_Engineer()
{
}

CState_Combat_GuardCancel_Engineer* CState_Combat_GuardCancel_Engineer::Create()
{
    CState_Combat_GuardCancel_Engineer* pInstance = new CState_Combat_GuardCancel_Engineer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardCancel_Engineer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardCancel_Engineer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDCANCEL_ENGINEER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 100;

    return S_OK;
}

void CState_Combat_GuardCancel_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardCancel_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
        return m_ePreStateType;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardCancel_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_GuardCancel_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{  
    return STATE_END;
}
