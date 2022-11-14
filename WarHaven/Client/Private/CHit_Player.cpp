#include "stdafx.h"
#include "CHit_Player.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_Player::CHit_Player()
{
}

CHit_Player::~CHit_Player()
{
}

CHit_Player* CHit_Player::Create()
{
    CHit_Player* pInstance = new CHit_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_Player::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 20;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

	m_vecAdjState.push_back(STATE_JUMPFALL_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_BOUNCE_PLAYER_R);


    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    Add_KeyFrame(30, 0);

    return S_OK;
}

void CHit_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	switch (ePrevType)
	{


	case Client::STATE_ATTACK_HORIZONTALMIDDLE_L:

		break;
	case Client::STATE_ATTACK_HORIZONTALMIDDLE_R:

		break;

	case Client::STATE_SPRINTATTACK_BEGIN_PLAYER:

		break;
	case Client::STATE_SPRINTATTACK_PLAYER:

		break;

	case Client::STATE_ATTACK_STING_PLAYER_L:

		break;

	case Client::STATE_ATTACK_STING_PLAYER_R:

		break;
	
	case Client::STATE_ATTACK_VERTICALCUT:

		break;

	case Client::STATE_WARRIOR_OXEN_LOOPATTACK:
		
		break;

	case Client::STATE_WARRIOR_GUARDBREAK:

		break;

	case Client::STATE_IDLE_WARRIOR_R_AI_ENEMY:

		break;

	case Client::STATE_END:

		break;
	default:

		break;
	}

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CHit_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bMoveTrigger)
		Move(Get_Direction(), pOwner);

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  LBuutton �� �̿��� �����Ѵ�.
    */
    if (KEY(CTRL, NONE))
    {
        if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
        {
            return m_eStateType;
        }
    }
   
    return STATE_END;
}

void CHit_Player::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
		m_bMoveTrigger = false;
        pOwner->Get_PhysicsCom()->Set_MaxSpeed(10.f);
        pOwner->Get_PhysicsCom()->Set_SpeedasMax();
        pOwner->Set_DirAsLook();
        break;


    }
}
