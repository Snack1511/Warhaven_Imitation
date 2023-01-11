#include "stdafx.h"
#include "CSprintAttack_Valkyrie_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CColorController.h"

CSprintAttack_Valkyrie_Begin::CSprintAttack_Valkyrie_Begin()
{
}

CSprintAttack_Valkyrie_Begin::~CSprintAttack_Valkyrie_Begin()
{
}

CSprintAttack_Valkyrie_Begin* CSprintAttack_Valkyrie_Begin::Create()
{
    CSprintAttack_Valkyrie_Begin* pInstance = new CSprintAttack_Valkyrie_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_Valkyrie_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_Valkyrie_Begin::Initialize()
{
    m_eAnimType = ANIM_ATTACK;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 18;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINTATTACK_BEGIN_VALKYRIE;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
    m_vecAdjState.push_back(STATE_SPRINTATTACK_VALKYRIE);


    return S_OK;
}

void CSprintAttack_Valkyrie_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
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

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 0.7f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprintAttack_Valkyrie_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	pMyPhysicsCom->Set_Accel(m_fMyAccel);

    Follow_MouseLook(pOwner);
    pMyPhysicsCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));

    return __super::Tick(pOwner, pAnimator);

}

void CSprintAttack_Valkyrie_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSprintAttack_Valkyrie_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Sprint�� ���� ����
    1. ������Ʈ�߿� ���� ��ư�� ������
    */

	if (KEY(LBUTTON, TAP))
		return m_eStateType;


    return STATE_END;
}
