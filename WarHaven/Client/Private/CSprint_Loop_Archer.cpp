#include "stdafx.h"
#include "CSprint_Loop_Archer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Loop_Archer::CSprint_Loop_Archer()
{
}

CSprint_Loop_Archer::~CSprint_Loop_Archer()
{
}

CSprint_Loop_Archer* CSprint_Loop_Archer::Create()
{
    CSprint_Loop_Archer* pInstance = new CSprint_Loop_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Loop_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Loop_Archer::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 46;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_LOOP_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.15f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 200.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    Add_KeyFrame(12, 0);
    Add_KeyFrame(26, 0);

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_SPRINT_END_ARCHER);
    m_vecAdjState.push_back(STATE_SPRINT_JUMP_ARCHER);

    return S_OK;
}

void CSprint_Loop_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SPRINT_BEGIN_ARCHER || ePrevType == STATE_SPRINT_LOOP_ARCHER)
        m_fInterPolationTime = 0.f;

    else
        m_fInterPolationTime = 0.1f;



	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_float4 vCamLook = pOwner->Get_FollowCamLook();
	vCamLook.y = 0.f;

	//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	//���� �����̴� ����
	pMyPhysicsCom->Set_Dir(vCamLook);

	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();

	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.3f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_Loop_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Is_Air())
        return STATE_SPRINT_JUMPFALL_ARCHER;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_float4 vCamLook = pOwner->Get_FollowCamLook();
    vCamLook.y = 0.f;
	_float4 vCamRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
    vCamRight.y = 0.f;

	_float4 vDir = pOwner->Get_FollowCamLook();


	_uint iFrame = pAnimator->Get_CurAnimFrame();

    _float4 vFinalDir = ZERO_VECTOR;

    if (KEY(W, HOLD))
    {
        vFinalDir += vCamLook;
    }
    if (KEY(A, HOLD))
    {
        vFinalDir -= vCamRight;
    }
    if (KEY(S, HOLD))
    {
        vFinalDir -= vCamLook;
    }
    if (KEY(D, HOLD))
    {
        vFinalDir += vCamRight;
    }

    if (vFinalDir.Is_Zero())
    {
        vFinalDir = pMyTransform->Get_World(WORLD_LOOK);
    }

    

    vFinalDir.Normalize();
	vDir.y = 0.f;

	pMyTransform->Set_LerpLook(vFinalDir, m_fMyMaxLerp);
    _float4 vUnitLook = pMyTransform->Get_MyWorld(WORLD_LOOK);
	pMyPhysicsCom->Set_Dir(vUnitLook);
	pMyPhysicsCom->Set_Accel(m_fMyAccel);



    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Loop_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CSprint_Loop_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����δ�.
    */
   

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

   
    return STATE_END;
}

void CSprint_Loop_Archer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
        break;
    }
}