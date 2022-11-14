#include "stdafx.h"
#include "CWarrior_Attack_Sting_R.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Sting_R::CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R::~CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R* CWarrior_Attack_Sting_R::Create()
{
    CWarrior_Attack_Sting_R* pInstance = new CWarrior_Attack_Sting_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Sting_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Sting_R::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 13;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_STING_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.4f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

	Add_KeyFrame(40, 0);
	Add_KeyFrame(42, 1);


    return S_OK;
}

void CWarrior_Attack_Sting_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWarrior_Attack_Sting_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Sting_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWarrior_Attack_Sting_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1. CTRL + LBuutton �� �̿��� �����Ѵ�.
    */

   
    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return m_eStateType;

    return STATE_END;
}


void CWarrior_Attack_Sting_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:

		m_bMoveTrigger = false;
		pOwner->Get_PhysicsCom()->Set_MaxSpeed(3.f);
		pOwner->Get_PhysicsCom()->Set_SpeedasMax();
		pOwner->Set_DirAsLook();
		break;

	case 1:
		m_bAttackTrigger = true;

		//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	default:
		break;
	}
}
