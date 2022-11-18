#include "stdafx.h"
#include "CRun_WarHammer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_WarHammer_R::CRun_WarHammer_R()
{
}

CRun_WarHammer_R::~CRun_WarHammer_R()
{
}

CRun_WarHammer_R* CRun_WarHammer_R::Create()
{
    CRun_WarHammer_R* pInstance = new CRun_WarHammer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_WarHammer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_WarHammer_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_WARHAMMER);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);



   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 29;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 30;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 31;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 32;
   
   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 44;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 45;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 46;
   
   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 33;


    return S_OK;
}

void CRun_WarHammer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_WarHammer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    if (pOwner->Is_Air())
        return STATE_JUMPFALL_WARHAMMER_R;

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

void CRun_WarHammer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_WarHammer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Walk로 오는 조건
    1. CTRL 을 누르지 않은 상태
    2. WASD 를 누른 상태
    */

	return __super::Check_Condition(pOwner, pAnimator);
   
}
