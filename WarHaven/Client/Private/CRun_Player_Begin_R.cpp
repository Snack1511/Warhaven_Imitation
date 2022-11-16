#include "stdafx.h"
#include "CRun_Player_Begin_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Player_Begin_R::CRun_Player_Begin_R()
{
}

CRun_Player_Begin_R::~CRun_Player_Begin_R()
{
}

CRun_Player_Begin_R* CRun_Player_Begin_R::Create()
{
    CRun_Player_Begin_R* pInstance = new CRun_Player_Begin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Player_Begin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Player_Begin_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 25;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_BEGIN_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_vecAdjState.push_back(STATE_JUMP_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);


    m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
    m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;

   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 22;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 23;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 24;

   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 25;
    

    return S_OK;
}

void CRun_Player_Begin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Player_Begin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(SPACE, TAP))
    {
        return STATE_JUMP_PLAYER_R;
    }


	if (pAnimator->Is_CurAnimFinished())
		return STATE_RUN_PLAYER_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        _uint* pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_PLAYER_R, (void*)pInt);
        return STATE_END;

    }


    return __super::Tick(pOwner, pAnimator);

}

void CRun_Player_Begin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_Player_Begin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(LSHIFT, NONE))
    {
        if (
            KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD)
            )
        {
            return m_eStateType;
        }
    }

   
    return STATE_END;
}