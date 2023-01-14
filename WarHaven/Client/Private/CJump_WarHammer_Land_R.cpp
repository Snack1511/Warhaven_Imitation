#include "stdafx.h"
#include "CJump_WarHammer_Land_R.h"


#include "UsefulHeaders.h"


CJump_WarHammer_Land_R::CJump_WarHammer_Land_R()
{
}

CJump_WarHammer_Land_R::~CJump_WarHammer_Land_R()
{
}

CJump_WarHammer_Land_R* CJump_WarHammer_Land_R::Create()
{
    CJump_WarHammer_Land_R* pInstance = new CJump_WarHammer_Land_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_WarHammer_Land_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_WarHammer_Land_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 20;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_WARHAMMER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
	//m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);

    Init_CommonState_Player();

    Add_KeyFrame(10, 0);

    return S_OK;
}

void CJump_WarHammer_Land_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;


    switch (ePrevType)
    {
    case Client::STATE_SPRINT_JUMP_WARHAMMER:
    case Client::STATE_SPRINT_JUMPFALL_WARHAMMER:
        pOwner->Enter_State(STATE_SPRINT_END_WARHAMMER);
        return;

        break;
    default:
        break;
    }

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_WarHammer_Land_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CJump_WarHammer_Land_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_WarHammer_Land_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}

void CJump_WarHammer_Land_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_LandingGround", CHANNEL_ENVIRONMENT);
        break;
    }
}