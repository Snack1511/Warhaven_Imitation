#include "stdafx.h"
#include "CWarrior_Attack_Takedown.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Takedown::CWarrior_Attack_Takedown()
{
}

CWarrior_Attack_Takedown::~CWarrior_Attack_Takedown()
{
}

CWarrior_Attack_Takedown* CWarrior_Attack_Takedown::Create()
{
    CWarrior_Attack_Takedown* pInstance = new CWarrior_Attack_Takedown();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Takedown::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 14;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_VERTICAL_CUT;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.3f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);


    return S_OK;
}

void CWarrior_Attack_Takedown::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType);
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
}

STATE_TYPE CWarrior_Attack_Takedown::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pOwner->Is_Weapon_R_Collision())
        return STATE_IDLE_PLAYER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Takedown::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */

    //Exit���� ������ ���ܳ�����
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CWarrior_Attack_Takedown::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1. CTRL + LBuutton �� �̿��� �����Ѵ�.
    */

   
    if (MOUSE_MOVE(MMS_WHEEL) < 0)
        return m_eStateType;

    return STATE_END;
}
