#include "stdafx.h"
#include "CSprintAttack_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CColorController.h"

CSprintAttack_Priest::CSprintAttack_Priest()
{
}

CSprintAttack_Priest::~CSprintAttack_Priest()
{
}

CSprintAttack_Priest* CSprintAttack_Priest::Create()
{
    CSprintAttack_Priest* pInstance = new CSprintAttack_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_Priest::Initialize()
{
	m_eAnimType = ANIM_ATTACK;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 42;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINTATTACK_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 17.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 70;

	m_vecAdjState.push_back(STATE_IDLE_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_PRIEST);
	m_vecAdjState.push_back(STATE_WALK_PRIEST);

	Init_AttackState_Priest();
	Init_CommonState_Player();

	Add_KeyFrame(20, 0);
	Add_KeyFrame(32, 1);
	Add_KeyFrame(30, 111);
	Add_KeyFrame(42, 222);


    return S_OK;
}

void CSprintAttack_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->TurnOn_TrailEffect(true);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintAttackSpeed);
	pMyPhysicsCom->Set_SpeedasMax();





	//���� �����ϱ�
	//*���� : ����ϰ��� Exit���� �ݵ�� 1�� �ǵ����ֱ�


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprintAttack_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	Follow_MouseLook(pOwner);
	pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));


    return __super::Tick(pOwner, pAnimator);
}

void CSprintAttack_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->TurnOn_TrailEffect(false);

	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

    /* �Ұž��� */
}

STATE_TYPE CSprintAttack_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PRIEST�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}

void	CSprintAttack_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	switch (iSequence)
	{
		//Attack Begin
	case 0:
	{
		pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.15f;

		cout << "Attack Begin " << endl;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		m_bAttackTrigger = true;
	}

	//Attack Done
	break;
	case 1:
	{

		pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
		pMyPhysicsCom->Set_SpeedasMax();

		pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);


		cout << "Attack End " << endl;
		m_bAttackTrigger = false;
		m_bTrigger = false;
		

		pOwner->TurnOn_TrailEffect(false);
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	}
		break;
	case 111:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;
	case 222:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;
	default:
		break;
	}
}
