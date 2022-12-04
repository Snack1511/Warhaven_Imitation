#include "stdafx.h"
#include "CWalk_Archer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Archer_L::CWalk_Archer_L()
{
}

CWalk_Archer_L::~CWalk_Archer_L()
{
}

CWalk_Archer_L* CWalk_Archer_L::Create()
{
    CWalk_Archer_L* pInstance = new CWalk_Archer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Archer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Archer_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 29;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_ARCHER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_IDLE_ARCHER_L);
    m_vecAdjState.push_back(STATE_RUN_ARCHER_L);
	m_vecAdjState.push_back(STATE_JUMP_ARCHER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);



	// ���ĺ� �� �ִϸ��̼� ����

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 28;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 29;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 35;

    


    return S_OK;
}

void CWalk_Archer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Archer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_JUMPFALL_ARCHER_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_ARCHER_L;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Archer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Archer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
