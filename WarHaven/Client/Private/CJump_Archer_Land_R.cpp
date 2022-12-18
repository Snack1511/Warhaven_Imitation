#include "stdafx.h"
#include "CJump_Archer_Land_R.h"


#include "UsefulHeaders.h"


CJump_Archer_Land_R::CJump_Archer_Land_R()
{
}

CJump_Archer_Land_R::~CJump_Archer_Land_R()
{
}

CJump_Archer_Land_R* CJump_Archer_Land_R::Create()
{
    CJump_Archer_Land_R* pInstance = new CJump_Archer_Land_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Archer_Land_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Archer_Land_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 17;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 25;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
	m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
	m_vecAdjState.push_back(STATE_WALK_ARCHER_R);

	m_vecAdjState.push_back(STATE_RUN_ARCHER_R);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);

    return S_OK;
}

void CJump_Archer_Land_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    

    //switch (ePrevType)
    //{
    //case Client::STATE_SPRINT_JUMP_ARCHER:
    //case Client::STATE_SPRINT_JUMPFALL_ARCHER:
    //    pOwner->Enter_State(STATE_SPRINT_END_ARCHER);
    //    return;

    //    break;
    //default:
    //    break;
    //}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
    pAnimator->Set_CurFrame(20);
}

STATE_TYPE CJump_Archer_Land_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Archer_Land_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Archer_Land_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
