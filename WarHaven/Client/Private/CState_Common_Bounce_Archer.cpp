#include "stdafx.h"
#include "CState_Common_Bounce_Archer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Common_Bounce_Archer::CState_Common_Bounce_Archer()
{
}

CState_Common_Bounce_Archer::~CState_Common_Bounce_Archer()
{
}

CState_Common_Bounce_Archer* CState_Common_Bounce_Archer::Create()
{
    CState_Common_Bounce_Archer* pInstance = new CState_Common_Bounce_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Bounce_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Bounce_Archer::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_BOUNCE_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    return S_OK;
}

void CState_Common_Bounce_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == AI_STATE_COMBAT_ATTACK_SHOOT_ARCHER ||
        ePrevType == AI_STATE_COMBAT_ATTACK_SHOOT_POISION_ARCHER ||
        ePrevType == AI_STATE_COMBAT_ATTACK_SHOOT_SNIPING_ARCHER)
    {
        m_bShoot = true;
        m_fInterPolationTime = 0.f;
    }


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bShoot)
        return m_ePreStateType;

    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEFAULT_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CState_Common_Bounce_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  �����ϴ� ���� ƨ���
    */

	return __super::Check_Condition(pOwner, pAnimator);

}