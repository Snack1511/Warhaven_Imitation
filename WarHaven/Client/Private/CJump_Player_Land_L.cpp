#include "CJump_Player_Land_L.h"
#include "CState.h"

#include "UsefulHeaders.h"

CJump_Player_Land_L::CJump_Player_Land_L()
{
}

CJump_Player_Land_L::~CJump_Player_Land_L()
{
}

CJump_Player_Land_L* CJump_Player_Land_L::Create()
{
    CJump_Player_Land_L* pInstance = new CJump_Player_Land_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Player_Land_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Player_Land_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_PLAYER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);
    
    

	m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    
    Init_CommonState_Player();

    return S_OK;
}

void CJump_Player_Land_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Player_Land_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CJump_Player_Land_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CJump_Player_Land_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}

void CJump_Player_Land_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_EFFECTS);
        break;


    default:
        break;
    }


}