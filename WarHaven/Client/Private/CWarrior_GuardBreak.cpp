#include "stdafx.h"
#include "CWarrior_GuardBreak.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

//CColorController::COLORDESC tColorDesc;

CWarrior_GuardBreak::CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak::~CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak* CWarrior_GuardBreak::Create()
{
    CWarrior_GuardBreak* pInstance = new CWarrior_GuardBreak();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_GuardBreak");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_GuardBreak::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 15;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_GUARDBREAK;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 65;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);

	m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);


	Add_KeyFrame(26, 1);
	Add_KeyFrame(54, 2);

    return S_OK;
}

void CWarrior_GuardBreak::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->On_Use(CUnit::SKILL2);

    /* Owner�� Animator Set Idle�� */
    m_fMaxSpeed = pOwner->Get_Status().fSprintAttackSpeed;
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_GuardBreak::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);
    pOwner->Set_DirAsLook();

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_GuardBreak::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CWarrior_GuardBreak::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  ��ų��ư �� �̿��� �����Ѵ�.
    */

    if (!pOwner->Can_Use(CUnit::SKILL2))
        return STATE_END;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::E)
        return m_eStateType;


    return STATE_END;
}


void CWarrior_GuardBreak::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 1:
        pOwner->Get_PhysicsCom()->Set_SpeedasMax();
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;


	case 2:
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}


}