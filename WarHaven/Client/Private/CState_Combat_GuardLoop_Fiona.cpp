#include "stdafx.h"
#include "CState_Combat_GuardLoop_Fiona.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardLoop_Fiona::CState_Combat_GuardLoop_Fiona()
{
}

CState_Combat_GuardLoop_Fiona::~CState_Combat_GuardLoop_Fiona()
{
}

CState_Combat_GuardLoop_Fiona* CState_Combat_GuardLoop_Fiona::Create()
{
    CState_Combat_GuardLoop_Fiona* pInstance = new CState_Combat_GuardLoop_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardLoop_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardLoop_Fiona::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 45;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDLOOP_FIONA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 30;

    return S_OK;
}

void CState_Combat_GuardLoop_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = random(0, 2);

	pOwner->Enable_GuardCollider(true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardLoop_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (Get_TargetLook_Length(pOwner) > 3.5f)
        return AI_STATE_COMBAT_DEAFULT_FIONA_R;

    pOwner->Set_LookToTarget();

   
   

    switch (m_iRand)
    {
    case 0:
            
        if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L;

    case 1:

        if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame + 20)
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R;


    case 2:

        if(pAnimator->Is_CurAnimFinished())
            return AI_STATE_COMBAT_DEAFULT_FIONA_R;

        break;

    default:
        break;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardLoop_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CState_Combat_GuardLoop_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
