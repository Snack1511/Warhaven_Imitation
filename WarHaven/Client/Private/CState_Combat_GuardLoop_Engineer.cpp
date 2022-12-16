#include "stdafx.h"
#include "CState_Combat_GuardLoop_Engineer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardLoop_Engineer::CState_Combat_GuardLoop_Engineer()
{
}

CState_Combat_GuardLoop_Engineer::~CState_Combat_GuardLoop_Engineer()
{
}

CState_Combat_GuardLoop_Engineer* CState_Combat_GuardLoop_Engineer::Create()
{
    CState_Combat_GuardLoop_Engineer* pInstance = new CState_Combat_GuardLoop_Engineer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardLoop_Engineer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardLoop_Engineer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDLOOP_ENGINEER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 30;

    return S_OK;
}

void CState_Combat_GuardLoop_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = random(0, 2);

	pOwner->Enable_GuardCollider(true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardLoop_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (Get_TargetLook_Length(pOwner) > 3.5f)
        return AI_STATE_COMBAT_DEFAULT_ENGINEER_R;

    pOwner->Set_LookToTarget();

   
   

    switch (m_iRand)
    {
    case 0:
            
        if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_L;

    case 1:

        if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame + 20)
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R;


    case 2:

        if(pAnimator->Is_CurAnimFinished())
            return AI_STATE_COMBAT_DEFAULT_ENGINEER_R;

        break;

    default:
        break;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardLoop_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CState_Combat_GuardLoop_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
