#include "stdafx.h"
#include "CState_Combat_GuardCancel_Fiona.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardCancel_Fiona::CState_Combat_GuardCancel_Fiona()
{
}

CState_Combat_GuardCancel_Fiona::~CState_Combat_GuardCancel_Fiona()
{
}

CState_Combat_GuardCancel_Fiona* CState_Combat_GuardCancel_Fiona::Create()
{
    CState_Combat_GuardCancel_Fiona* pInstance = new CState_Combat_GuardCancel_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardCancel_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardCancel_Fiona::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 47;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDCANCEL_FIONA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 100;

    return S_OK;
}

void CState_Combat_GuardCancel_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == AI_STATE_COMBAT_COUNTER_FIONA)
        m_bAttackTrigger = true;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardCancel_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bAttackTrigger)
        return AI_STATE_COMBAT_SPINATTACK_FIONA;

	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
        return m_ePreStateType;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardCancel_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_GuardCancel_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{  
    return STATE_END;
}
