#include "stdafx.h"
#include "CWalk_SpearMan_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_SpearMan_L::CWalk_SpearMan_L()
{
}

CWalk_SpearMan_L::~CWalk_SpearMan_L()
{
}

CWalk_SpearMan_L* CWalk_SpearMan_L::Create()
{
    CWalk_SpearMan_L* pInstance = new CWalk_SpearMan_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_SpearMan_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_SpearMan_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
	m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_L);
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_L);
	m_vecAdjState.push_back(STATE_RUN_SPEARMAN_L);
	
	//   m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
 //   m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
	//m_vecAdjState.push_back(STATE_JUMP_PLAYER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);

	//m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);

	
	// �ִϸ��̼� ���ĺ� �� ����
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

void CWalk_SpearMan_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWalk_SpearMan_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PLAYER_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_PLAYER_L;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CWalk_SpearMan_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_SpearMan_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
   
        //CTRL �� �ٲټ�.
      
      

   


    return STATE_END;
}
