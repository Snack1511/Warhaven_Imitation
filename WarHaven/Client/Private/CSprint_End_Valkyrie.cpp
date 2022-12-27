#include "stdafx.h"
#include "CSprint_End_Valkyrie.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_End_Valkyrie::CSprint_End_Valkyrie()
{
}

CSprint_End_Valkyrie::~CSprint_End_Valkyrie()
{
}

CSprint_End_Valkyrie* CSprint_End_Valkyrie::Create()
{
    CSprint_End_Valkyrie* pInstance = new CSprint_End_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_End_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_End_Valkyrie::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 46;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_END_VALKYRIE;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    
    
    m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_L);

    m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
    m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
    m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);


    Init_CommonState_Hero_Player();


	m_fMyMaxLerp = 0.4f;
	m_fMaxSpeed = 10.f;
	m_fMyAccel = 10.f;

    Add_KeyFrame(7, 222);
    Add_KeyFrame(30, 333);
    return S_OK;
}

void CSprint_End_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SPRINT_LOOP_VALKYRIE)
        m_fInterPolationTime = 0.f;
    else
        m_fInterPolationTime = 0.05f;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_End_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Is_Air())
        return STATE_SPRINT_JUMPFALL_VALKYRIE;

    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_VALKYRIE_R;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom(); 


    return __super::Tick(pOwner, pAnimator);

}

void CSprint_End_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CSprint_End_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����δ�.
    */
    // ���� ����ƮŰ �� W �� �������� ��
    if (KEY(LSHIFT, NONE) ||
        (KEY(W, NONE) && KEY(A, NONE) && KEY(S, NONE) && KEY(D, NONE)))
        return STATE_SPRINT_END_VALKYRIE;

    return STATE_END;
}

void CSprint_End_Valkyrie::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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
