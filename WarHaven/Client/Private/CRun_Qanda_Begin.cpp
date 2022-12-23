#include "stdafx.h"
#include "CRun_Qanda_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Qanda_Begin::CRun_Qanda_Begin()
{
}

CRun_Qanda_Begin::~CRun_Qanda_Begin()
{
}

CRun_Qanda_Begin* CRun_Qanda_Begin::Create()
{
    CRun_Qanda_Begin* pInstance = new CRun_Qanda_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Qanda_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Qanda_Begin::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 25;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUNBEGIN_ARCHER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_JUMP_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUN_ARCHER_R);


    //m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
    //m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);

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

void CRun_Qanda_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    _uint iDirection = Get_Direction();

    DoMove(iDirection, pOwner);


    if (iDirection < 8)
        m_iCurDirection = iDirection;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Qanda_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(SPACE, TAP))
    {
        return STATE_JUMP_ARCHER_R;
    }


	if (pAnimator->Is_CurAnimFinished())
		return STATE_RUN_ARCHER_R;

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

void CRun_Qanda_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Qanda_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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