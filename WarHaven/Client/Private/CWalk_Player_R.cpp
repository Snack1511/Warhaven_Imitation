#include "stdafx.h"
#include "CWalk_Player_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Player_R::CWalk_Player_R()
{
}

CWalk_Player_R::~CWalk_Player_R()
{
}

CWalk_Player_R* CWalk_Player_R::Create()
{
    CWalk_Player_R* pInstance = new CWalk_Player_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Player_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Player_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.

    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	m_vecAdjState.push_back(STATE_JUMP_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);


//    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 45;
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 38;

                            



    return S_OK;
}

void CWalk_Player_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Player_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PLAYER_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_PLAYER_R;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Player_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Player_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. 
    */

    // m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

    // W �� A �� ������ ���� ������ �̵��Ѵ�.
    // W �� D �� ������ ���� ������ �̵��Ѵ�.

    // ���� WASD �� �����ٸ�

    // ������ ���� �ʰ�
    if (KEY(SPACE, NONE))
    {
        // õõ�� 
        if (KEY(CTRL, HOLD))
        {
            // �ɾ��.
            if (KEY(W, HOLD) ||
                KEY(A, HOLD) ||
                KEY(S, HOLD) ||
                KEY(D, HOLD))
            {

                return m_eStateType;
            }

        }
    }

   


    return STATE_END;
}
