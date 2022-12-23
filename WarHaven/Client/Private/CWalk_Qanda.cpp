#include "stdafx.h"
#include "CWalk_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Qanda::CWalk_Qanda()
{
}

CWalk_Qanda::~CWalk_Qanda()
{
}

CWalk_Qanda* CWalk_Qanda::Create()
{
    CWalk_Qanda* pInstance = new CWalk_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Qanda::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 39;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.


    m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
	m_vecAdjState.push_back(STATE_JUMP_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUN_ARCHER_R);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_R);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

	//m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);


//    m_vecAdjState.push_back(STATE_JUMP_ARCHER);

	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 38;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 45;

                            



    return S_OK;
}

void CWalk_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */
    m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Move_Direction_Loop(pOwner, pAnimator, 0.05f);

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_ARCHER_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_ARCHER_R;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(C, HOLD))
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

    return STATE_END;
}
