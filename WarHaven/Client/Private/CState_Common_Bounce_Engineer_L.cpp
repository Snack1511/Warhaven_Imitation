#include "stdafx.h"
#include "CState_Common_Bounce_Engineer_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Common_Bounce_Engineer_L::CState_Common_Bounce_Engineer_L()
{
}

CState_Common_Bounce_Engineer_L::~CState_Common_Bounce_Engineer_L()
{
}

CState_Common_Bounce_Engineer_L* CState_Common_Bounce_Engineer_L::Create()
{
    CState_Common_Bounce_Engineer_L* pInstance = new CState_Common_Bounce_Engineer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Bounce_Engineer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Bounce_Engineer_L::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_BOUNCE_ENGINEER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    return S_OK;
}

void CState_Common_Bounce_Engineer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce_Engineer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEAFULT_ENGINEER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce_Engineer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CState_Common_Bounce_Engineer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}