#include "stdafx.h"
#include "CSprint_End.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_End::CSprint_End()
{
}

CSprint_End::~CSprint_End()
{
}

CSprint_End* CSprint_End::Create()
{
    CSprint_End* pInstance = new CSprint_End();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_End");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_End::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 56;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_END_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
	m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);


	m_fMyMaxLerp = 0.4f;
	m_fMaxSpeed = 10.f;
	m_fMyAccel = 10.f;

    Add_KeyFrame(18, 222);
    Add_KeyFrame(29, 333);
    return S_OK;
}

void CSprint_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SPRINT_LOOP_PLAYER)
        m_fInterPolationTime = 0.f;
    else
        m_fInterPolationTime = 0.05f;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	//vCamLook.y = 0.f;

	//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	//pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	//���� �����̴� ����
	//pMyPhysicsCom->Set_Dir(vCamLook);

	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Is_Air())
        return STATE_SPRINT_JUMPFALL_PLAYER;

    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PLAYER_R;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom(); 


    return __super::Tick(pOwner, pAnimator);

}

void CSprint_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CSprint_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����δ�.
    */
    // ���� ����ƮŰ �� W �� �������� ��
    if (KEY(LSHIFT, NONE) ||
        (KEY(W, NONE) && KEY(A, NONE) && KEY(S, NONE) && KEY(D, NONE)))
        return STATE_SPRINT_END_PLAYER;

    return STATE_END;
}

void CSprint_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 222:
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    case 333:
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    default:
        break;
    }
}
