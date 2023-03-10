#include "stdafx.h"
#include "CRun_WarHammer_Begin_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_WarHammer_Begin_R::CRun_WarHammer_Begin_R()
{
}

CRun_WarHammer_Begin_R::~CRun_WarHammer_Begin_R()
{
}

CRun_WarHammer_Begin_R* CRun_WarHammer_Begin_R::Create()
{
    CRun_WarHammer_Begin_R* pInstance = new CRun_WarHammer_Begin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_WarHammer_Begin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_WarHammer_Begin_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 22;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUNBEGIN_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_WARHAMMER);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);


   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 21;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 22;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 23;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 24;

   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 25;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 26;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 27;

   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 28;
    

    return S_OK;
}

void CRun_WarHammer_Begin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_WarHammer_Begin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(SPACE, TAP))
    {
        return STATE_JUMP_WARHAMMER_R;
    }


	if (pAnimator->Is_CurAnimFinished())
		return STATE_RUN_WARHAMMER_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        _uint* pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_WARHAMMER_R, (void*)pInt);
        return STATE_END;

    }


    return __super::Tick(pOwner, pAnimator);

}

void CRun_WarHammer_Begin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_WarHammer_Begin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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