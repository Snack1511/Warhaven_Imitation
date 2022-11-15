#include "stdafx.h"
#include "CWalk_SpearMan_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_SpearMan_R::CWalk_SpearMan_R()
{
}

CWalk_SpearMan_R::~CWalk_SpearMan_R()
{
}

CWalk_SpearMan_R* CWalk_SpearMan_R::Create()
{
    CWalk_SpearMan_R* pInstance = new CWalk_SpearMan_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_SpearMan_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_SpearMan_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_SPEARMAN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.

    m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);


	//m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN_R);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

	// m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);


//    m_vecAdjState.push_back(STATE_JUMP_SPEARMAN);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

	// ���ĺ� �� �ִϸ��̼� ����
	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 37;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 38;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 39;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 40;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 41;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 42;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 43;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 44;
                            
    
    
    
	



    return S_OK;
}

void CWalk_SpearMan_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWalk_SpearMan_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_SPEARMAN_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_SPEARMAN_R;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_SpearMan_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_SpearMan_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN�� Walk�� ���� ����
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
