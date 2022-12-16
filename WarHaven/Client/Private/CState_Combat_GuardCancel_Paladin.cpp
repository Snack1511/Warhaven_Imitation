#include "stdafx.h"
#include "CState_Combat_GuardCancel_Paladin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardCancel_Paladin::CState_Combat_GuardCancel_Paladin()
{
}

CState_Combat_GuardCancel_Paladin::~CState_Combat_GuardCancel_Paladin()
{
}

CState_Combat_GuardCancel_Paladin* CState_Combat_GuardCancel_Paladin::Create()
{
    CState_Combat_GuardCancel_Paladin* pInstance = new CState_Combat_GuardCancel_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardCancel_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardCancel_Paladin::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 47;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDCANCEL_PALADIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 100;

    return S_OK;
}

void CState_Combat_GuardCancel_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardCancel_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
    {
        if (m_ePreStateType != STATE_END)
            return m_ePreStateType;
        else
            return AI_STATE_COMBAT_DEFAULT_PALADIN_R;
    }
        

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardCancel_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_GuardCancel_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{  
    return STATE_END;
}
