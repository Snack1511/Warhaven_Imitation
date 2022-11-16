#include "stdafx.h"
#include "CSprintAttack_Player_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprintAttack_Player_Begin::CSprintAttack_Player_Begin()
{
}

CSprintAttack_Player_Begin::~CSprintAttack_Player_Begin()
{
}

CSprintAttack_Player_Begin* CSprintAttack_Player_Begin::Create()
{
    CSprintAttack_Player_Begin* pInstance = new CSprintAttack_Player_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_Player_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_Player_Begin::Initialize()
{
    m_eAnimType = ANIM_ATTACK;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 20;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINTATTACK_BEGIN_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
    m_vecAdjState.push_back(STATE_SPRINTATTACK_PLAYER);


    return S_OK;
}

void CSprintAttack_Player_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//�ӽ�
	pMyPhysicsCom->Get_Physics().bAir = false;

	_float4 vCamLook = pOwner->Get_FollowCamLook();
	vCamLook.y = 0.f;

	//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	//���� �����̴� ����
	pMyPhysicsCom->Set_Dir(vCamLook);

	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprintAttack_Player_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	pMyPhysicsCom->Set_Accel(m_fMyAccel);

    Follow_MouseLook(pOwner);
    pMyPhysicsCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));

    return __super::Tick(pOwner, pAnimator);

}

void CSprintAttack_Player_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSprintAttack_Player_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
    1. ������Ʈ�߿� ���� ��ư�� ������
    */

	if (KEY(LBUTTON, TAP))
		return m_eStateType;


    return STATE_END;
}
