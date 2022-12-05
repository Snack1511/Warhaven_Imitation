#include "stdafx.h"
#include "CRun_Archer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Archer_R::CRun_Archer_R()
{
}

CRun_Archer_R::~CRun_Archer_R()
{
}

CRun_Archer_R* CRun_Archer_R::Create()
{
    CRun_Archer_R* pInstance = new CRun_Archer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Archer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Archer_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 27;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
    m_vecAdjState.push_back(STATE_JUMP_ARCHER_R);
  /*  m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);

	m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);*/


   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 26;
   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 27;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 28;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 29;
   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 30;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 31;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 32;


    return S_OK;
}

void CRun_Archer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Archer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    if (pOwner->Is_Air())
        return STATE_JUMPFALL_ARCHER_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        _uint* pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_ARCHER_R, (void*)pInt);
        return STATE_END;

    }

    

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Archer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_Archer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. CTRL �� ������ ���� ����
    2. WASD �� ���� ����
    */

	return __super::Check_Condition(pOwner, pAnimator);
   
}
