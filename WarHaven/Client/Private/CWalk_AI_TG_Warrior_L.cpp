#include "stdafx.h"
#include "CWalk_AI_TG_Warrior_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_AI_TG_Warrior_L::CWalk_AI_TG_Warrior_L()
{
}

CWalk_AI_TG_Warrior_L::~CWalk_AI_TG_Warrior_L()
{
}

CWalk_AI_TG_Warrior_L* CWalk_AI_TG_Warrior_L::Create()
{
    CWalk_AI_TG_Warrior_L* pInstance = new CWalk_AI_TG_Warrior_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_AI_TG_Warrior_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_AI_TG_Warrior_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_WALK_WARRIOR_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
 //   m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
 //   m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
	//m_vecAdjState.push_back(STATE_JUMP_PLAYER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);

	//m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);

	// ���ĺ� �� �ִϸ��̼� ����

    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 30;


    


    return S_OK;
}

void CWalk_AI_TG_Warrior_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_AI_TG_Warrior_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    
    _float fLength = Get_TargetLook_Length(pOwner);

    if (fLength < 2.5f)
        return AI_STATE_ATTACK_HORIZONTALMIDDLE_L;


    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_iRand == 0)
            return AI_STATE_RUN_WARRIOR_L;
        else
            --m_iRand;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CWalk_AI_TG_Warrior_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_AI_TG_Warrior_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
