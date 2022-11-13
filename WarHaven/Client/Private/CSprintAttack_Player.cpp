#include "stdafx.h"
#include "CSprintAttack_Player.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

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
	m_eAnimType = ANIM_ATTACK;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 21;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINTATTACK_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 17.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 80;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);


    return S_OK;
}

void CSprintAttack_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//�ӽ�
	pMyPhysicsCom->Get_Physics().bAir = false;

	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	vCamLook.y = 0.f;

	//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	pMyTransform->Set_LerpLook(vCamLook, 0.4f);

	//���� �����̴� ����
	pMyPhysicsCom->Set_Dir(vCamLook);

	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintAttackSpeed);
	pMyPhysicsCom->Set_SpeedasMax();


	//���� �����ϱ�
	//*���� : ����ϰ��� Exit���� �ݵ�� 1�� �ǵ����ֱ�
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.65f;
	pMyPhysicsCom->Set_Accel(m_fMyAccel);

	Add_KeyFrame(33, 1);

	//���� �����ϱ�
	//*���� : ����ϰ��� Exit���� �ݵ�� 1�� �ǵ����ֱ�


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CSprintAttack_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (33 > pAnimator->Get_CurAnimFrame())
	{
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Set_Accel(m_fMyAccel);
	}
	

    return __super::Tick(pOwner, pAnimator);

}

void CSprintAttack_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
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

void	CSprintAttack_Player::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 1:
	{
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.7f;

	}


	default:
		break;
	}
}
