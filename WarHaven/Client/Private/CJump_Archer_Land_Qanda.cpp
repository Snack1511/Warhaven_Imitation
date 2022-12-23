#include "stdafx.h"
#include "CJump_Archer_Land_Qanda.h"


#include "UsefulHeaders.h"


CJump_Archer_Land_Qanda::CJump_Archer_Land_Qanda()
{
}

CJump_Archer_Land_Qanda::~CJump_Archer_Land_Qanda()
{
}

CJump_Archer_Land_Qanda* CJump_Archer_Land_Qanda::Create()
{
    CJump_Archer_Land_Qanda* pInstance = new CJump_Archer_Land_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Archer_Land_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Archer_Land_Qanda::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 7;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.15f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
	m_vecAdjState.push_back(STATE_IDLE_QANDA);
	m_vecAdjState.push_back(STATE_WALK_QANDA);

	m_vecAdjState.push_back(STATE_RUN_QANDA);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);

    Init_CommonState_Player();

    return S_OK;
}

void CJump_Archer_Land_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
    pAnimator->Set_CurFrame(20);
}

STATE_TYPE CJump_Archer_Land_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_QANDA;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Archer_Land_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Archer_Land_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
