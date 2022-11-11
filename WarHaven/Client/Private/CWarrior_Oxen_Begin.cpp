#include "stdafx.h"
#include "CWarrior_Oxen_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen_Begin::CWarrior_Oxen_Begin()
{
}

CWarrior_Oxen_Begin::~CWarrior_Oxen_Begin()
{
}

CWarrior_Oxen_Begin* CWarrior_Oxen_Begin::Create()
{
    CWarrior_Oxen_Begin* pInstance = new CWarrior_Oxen_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen_Begin::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 24;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_BEGIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.2f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_LOOPATTACK);

    return S_OK;
}

void CWarrior_Oxen_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    pOwner->TurnOn_TrailEffect(true);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWarrior_Oxen_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    //�ִϸ��̼� ������ Loop��
    if (pAnimator->Is_CurAnimFinished())
        return STATE_WARRIOR_OXEN_LOOP;

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CWarrior_Oxen_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Oxen_Begin ���� ���� ����
    1.  Q �� �̿��� �����Ѵ�.
    2.  ��Ÿ���� ����.
        */

    if(KEY(Q, TAP))
        return m_eStateType;

    return STATE_END;
}

