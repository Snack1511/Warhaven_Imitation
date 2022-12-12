#include "stdafx.h"
#include "CState_Combat_GuardBegin_Fiona.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardBegin_Fiona::CState_Combat_GuardBegin_Fiona()
{
}

CState_Combat_GuardBegin_Fiona::~CState_Combat_GuardBegin_Fiona()
{
}

CState_Combat_GuardBegin_Fiona* CState_Combat_GuardBegin_Fiona::Create()
{
    CState_Combat_GuardBegin_Fiona* pInstance = new CState_Combat_GuardBegin_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardBegin_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardBegin_Fiona::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDBEGIN_FIONA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;


    return S_OK;
}

void CState_Combat_GuardBegin_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_iRand = random(0, 2);


	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardBegin_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    pOwner->Set_LookToTarget();

    if (m_iRand == 0)
    {
        if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
        {
            if (Get_TargetLook_Length(pOwner) > 3.f)
               return AI_STATE_COMBAT_DEAFULT_FIONA_R;

            else
                return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R;
        }
    }
    else
    {
        if (pAnimator->Is_CurAnimFinished())
        {
            return AI_STATE_COMBAT_GUARDLOOP_FIONA;
        }
    }

    return __super::Tick(pOwner, pAnimator);

}

void CState_Combat_GuardBegin_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CState_Combat_GuardBegin_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
   
    return STATE_END;
}
