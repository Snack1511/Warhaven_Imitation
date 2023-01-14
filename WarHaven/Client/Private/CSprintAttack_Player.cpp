#include "stdafx.h"
#include "CSprintAttack_Player.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CColorController.h"

CSprintAttack_Player::CSprintAttack_Player()
{
}

CSprintAttack_Player::~CSprintAttack_Player()
{
}

CSprintAttack_Player* CSprintAttack_Player::Create()
{
    CSprintAttack_Player* pInstance = new CSprintAttack_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_Player::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 4.f;
	m_tHitInfo.fJumpPower = 0.f;

	m_eAnimType = ANIM_ATTACK;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 21;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINTATTACK_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 17.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 70;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

	m_eBounceState = STATE_BOUNCE_PLAYER_L;

	Add_KeyFrame(23, 0);
	Add_KeyFrame(32, 1);


    return S_OK;
}

void CSprintAttack_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->TurnOn_TrailEffect(true);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();



	pOwner->Set_BounceState(STATE_BOUNCE_PLAYER_L);


	//pOwner->CallBack_CollisionEnter += bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);


	//���� �����ϱ�
	//*���� : ����ϰ��� Exit���� �ݵ�� 1�� �ǵ����ֱ�

	Add_KeyFrame(42, 444);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprintAttack_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (m_bTrigger)
	{
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Set_Accel(m_fMyAccel);
	}
	
	Follow_MouseLook(pOwner);
	pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));


    return __super::Tick(pOwner, pAnimator);

}

void CSprintAttack_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

	pOwner->TurnOn_TrailEffect(false);

	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

    /* �Ұž��� */
}

STATE_TYPE CSprintAttack_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}

void CSprintAttack_Player::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{

}

void	CSprintAttack_Player::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
		//Attack Begin
	case 0:
	{
		Play_Sound(L"Effect_Swing_Warrior", CHANNEL_EFFECTS);
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		m_bAttackTrigger = true;
	}

	//Attack Done
	break;
	case 1:
	{
		m_bAttackTrigger = false;
		m_bTrigger = false;
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.7f;
		pOwner->TurnOn_TrailEffect(false);
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	}

	break;

	case 444:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;
	default:
		break;
	}
}
