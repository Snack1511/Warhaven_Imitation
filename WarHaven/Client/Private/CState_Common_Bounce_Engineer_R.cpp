#include "stdafx.h"
#include "CState_Common_Bounce_Engineer_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Common_Bounce_Engineer_R::CState_Common_Bounce_Engineer_R()
{
}

CState_Common_Bounce_Engineer_R::~CState_Common_Bounce_Engineer_R()
{
}

CState_Common_Bounce_Engineer_R* CState_Common_Bounce_Engineer_R::Create()
{
    CState_Common_Bounce_Engineer_R* pInstance = new CState_Common_Bounce_Engineer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Bounce_Engineer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Bounce_Engineer_R::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 10;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_BOUNCE_ENGINEER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    return S_OK;
}

void CState_Common_Bounce_Engineer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == AI_STATE_PATROL_SWITCH_R_TO_L)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce_Engineer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEAFULT_ENGINEER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce_Engineer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CState_Common_Bounce_Engineer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  �����ϴ� ���� ƨ���
    */

	return __super::Check_Condition(pOwner, pAnimator);

}