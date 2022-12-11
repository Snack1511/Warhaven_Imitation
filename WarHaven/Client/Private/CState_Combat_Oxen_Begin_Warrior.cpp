#include "stdafx.h"
#include "CState_Combat_Oxen_Begin_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"
#include "Model.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"
#include "HIerarchyNode.h"

CState_Combat_Oxen_Begin_Warrior::CState_Combat_Oxen_Begin_Warrior()
{
}

CState_Combat_Oxen_Begin_Warrior::~CState_Combat_Oxen_Begin_Warrior()
{
}

CState_Combat_Oxen_Begin_Warrior* CState_Combat_Oxen_Begin_Warrior::Create()
{
    CState_Combat_Oxen_Begin_Warrior* pInstance = new CState_Combat_Oxen_Begin_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Oxen_Begin_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Oxen_Begin_Warrior::Initialize()
{


    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 24;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_BEGIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.2f;

    return S_OK;
}

void CState_Combat_Oxen_Begin_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->TurnOn_TrailEffect(true);

 
    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Oxen_Begin_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    //�ִϸ��̼� ������ Loop��
    if (pAnimator->Is_CurAnimFinished())
        return STATE_WARRIOR_OXEN_LOOP;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Oxen_Begin_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CState_Combat_Oxen_Begin_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Oxen_Begin ���� ���� ����
    1.  Q �� �̿��� �����Ѵ�.
     */

    if (!pOwner->Can_Use(CUnit::SKILL2))
        return STATE_END;

    if(KEY(Q, TAP))
        return m_eStateType;

    return STATE_END;
}

