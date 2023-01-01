#include "stdafx.h"
#include "CJump_Lancer_Land.h"


#include "UsefulHeaders.h"


CJump_Lancer_Land::CJump_Lancer_Land()
{
}

CJump_Lancer_Land::~CJump_Lancer_Land()
{
}

CJump_Lancer_Land* CJump_Lancer_Land::Create()
{
    CJump_Lancer_Land* pInstance = new CJump_Lancer_Land();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Lancer_Land");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Lancer_Land::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 8;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
	m_vecAdjState.push_back(STATE_IDLE_LANCER);
	m_vecAdjState.push_back(STATE_RUN_LANCER);

    return S_OK;
}

void CJump_Lancer_Land::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Lancer_Land::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_LANCER;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Lancer_Land::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Lancer_Land::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
