#include "stdafx.h"
#include "CJump_Paladin_Land_R.h"


#include "UsefulHeaders.h"


CJump_Paladin_Land_R::CJump_Paladin_Land_R()
{
}

CJump_Paladin_Land_R::~CJump_Paladin_Land_R()
{
}

CJump_Paladin_Land_R* CJump_Paladin_Land_R::Create()
{
    CJump_Paladin_Land_R* pInstance = new CJump_Paladin_Land_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Paladin_Land_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Paladin_Land_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_PALADIN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
	//m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

	m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
	m_vecAdjState.push_back(STATE_WALK_PALADIN_R);

	m_vecAdjState.push_back(STATE_RUN_PALADIN_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);

    Init_CommonState_Player();

    return S_OK;
}

void CJump_Paladin_Land_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    switch (ePrevType)
    {
    case Client::STATE_SPRINT_JUMP_PALADIN:
    case Client::STATE_SPRINT_JUMPFALL_PALADIN:
        pOwner->Enter_State(STATE_SPRINT_END_PALADIN);
        return;

        break;
    default:
        break;
    }


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Paladin_Land_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PALADIN_R;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Paladin_Land_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Paladin_Land_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
