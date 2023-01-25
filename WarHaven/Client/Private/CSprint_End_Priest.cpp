#include "stdafx.h"
#include "CSprint_End_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_End_Priest::CSprint_End_Priest()
{
}

CSprint_End_Priest::~CSprint_End_Priest()
{
}

CSprint_End_Priest* CSprint_End_Priest::Create()
{
	CSprint_End_Priest* pInstance = new CSprint_End_Priest();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CSprint_End_Priest");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CSprint_End_Priest::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 44;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINT_END_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_iStateChangeKeyFrame = 30;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.05f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;

	// Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

	//enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.

	m_vecAdjState.push_back(STATE_IDLE_PRIEST);
	m_vecAdjState.push_back(STATE_WALK_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_PRIEST);

	Init_AttackState_Priest();
	Init_CommonState_Player();

	m_fMyMaxLerp = 0.4f;
	m_fMaxSpeed = 10.f;
	m_fMyAccel = 10.f;

	Add_KeyFrame(22, 0, true);
	Add_KeyFrame(31, 0, true);

	return S_OK;
}

void CSprint_End_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	if (ePrevType == STATE_SPRINT_LOOP_PRIEST)
		m_fInterPolationTime = 0.f;
	else
		m_fInterPolationTime = 0.05f;


	if (ePrevType == STATE_GLIDING)
		m_fInterPolationTime = 0.2f;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_End_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_PRIEST;

	return __super::Tick(pOwner, pAnimator);
}

void CSprint_End_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
	/* �Ұž��� */
}

STATE_TYPE CSprint_End_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* PRIEST�� Sprint�� ���� ����
	1. ����Ʈ�� ���� ���¿��� �����δ�.
	*/
	// ���� ����ƮŰ �� W �� �������� ��
	if (KEY(LSHIFT, NONE) ||
		(KEY(W, NONE) && KEY(A, NONE) && KEY(S, NONE) && KEY(D, NONE)))
		return STATE_SPRINT_END_PRIEST;

	return STATE_END;
}

void CSprint_End_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
		break;

	default:
		break;
	}


}