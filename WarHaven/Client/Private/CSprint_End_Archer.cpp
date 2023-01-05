#include "stdafx.h"
#include "CSprint_End_Archer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_End_Archer::CSprint_End_Archer()
{
}

CSprint_End_Archer::~CSprint_End_Archer()
{
}

CSprint_End_Archer* CSprint_End_Archer::Create()
{
    CSprint_End_Archer* pInstance = new CSprint_End_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_End_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_End_Archer::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 48;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_END_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_GUARD_BEGIN_ARCHER);
    
    m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
    m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUN_ARCHER_R);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_POISION_ARCHER);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);

    Init_CommonState_Player();

	m_fMyMaxLerp = 0.4f;
	m_fMaxSpeed = 10.f;
	m_fMyAccel = 10.f;

    Add_KeyFrame(18, 111);
    Add_KeyFrame(28, 222);

    return S_OK;
}

void CSprint_End_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SPRINT_LOOP_ARCHER)
        m_fInterPolationTime = 0.f;
    else
        m_fInterPolationTime = 0.05f;

    if (ePrevType == STATE_GLIDING_ARCHER)
        m_fInterPolationTime = 0.2f;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_End_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom(); 

    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CSprint_End_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CSprint_End_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����δ�.
    */
    // ���� ����ƮŰ �� W �� �������� ��
    if (KEY(LSHIFT, NONE) ||
        (KEY(W, NONE) && KEY(A, NONE) && KEY(S, NONE) && KEY(D, NONE)))
        return STATE_SPRINT_END_ARCHER;

    return STATE_END;
}

void CSprint_End_Archer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 111:
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    case 222:
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    default:
        break;
    }
}
