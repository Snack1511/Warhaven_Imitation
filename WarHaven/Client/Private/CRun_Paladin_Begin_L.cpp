#include "stdafx.h"
#include "CRun_Paladin_Begin_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Paladin_Begin_L::CRun_Paladin_Begin_L()
{
}

CRun_Paladin_Begin_L::~CRun_Paladin_Begin_L()
{
}

CRun_Paladin_Begin_L* CRun_Paladin_Begin_L::Create()
{
    CRun_Paladin_Begin_L* pInstance = new CRun_Paladin_Begin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Paladin_Begin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Paladin_Begin_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 11;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUNBEGIN_PALADIN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_PALADIN);
    m_vecAdjState.push_back(STATE_JUMP_PALADIN_L);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_PALADIN_L);



    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 10;

    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 11;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 12;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 13;

    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 15;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 16;

    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 17;

    return S_OK;
}

void CRun_Paladin_Begin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Paladin_Begin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(SPACE, TAP))
    {
        return STATE_JUMP_PALADIN_L;
    }

	if (pAnimator->Is_CurAnimFinished())
		return STATE_RUN_PALADIN_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        _uint* pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_PALADIN_L, (void*)pInt);
        return STATE_END;

    }


    return __super::Tick(pOwner, pAnimator);

}

void CRun_Paladin_Begin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Paladin_Begin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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