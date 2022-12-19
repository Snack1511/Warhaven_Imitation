#include "CJump_Archer_Land_L.h"
#include "CState.h"

#include "UsefulHeaders.h"

CJump_Archer_Land_L::CJump_Archer_Land_L()
{
}

CJump_Archer_Land_L::~CJump_Archer_Land_L()
{
}

CJump_Archer_Land_L* CJump_Archer_Land_L::Create()
{
    CJump_Archer_Land_L* pInstance = new CJump_Archer_Land_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Archer_Land_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Archer_Land_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 8;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_ARCHER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.15f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;


	m_vecAdjState.push_back(STATE_IDLE_ARCHER_L);
	m_vecAdjState.push_back(STATE_WALK_ARCHER_L);
	m_vecAdjState.push_back(STATE_RUN_ARCHER_L);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);

    


    return S_OK;
}

void CJump_Archer_Land_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
    pAnimator->Set_CurFrame(10);
}

STATE_TYPE CJump_Archer_Land_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_ARCHER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Archer_Land_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CJump_Archer_Land_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
