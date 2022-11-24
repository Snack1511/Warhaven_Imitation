#include "stdafx.h"
#include "CRun_Valkyrie_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Valkyrie_R::CRun_Valkyrie_R()
{
}

CRun_Valkyrie_R::~CRun_Valkyrie_R()
{
}

CRun_Valkyrie_R* CRun_Valkyrie_R::Create()
{
    CRun_Valkyrie_R* pInstance = new CRun_Valkyrie_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Valkyrie_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Valkyrie_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_VALKYRIE_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_VALKYRIE);


    m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;

    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;

    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 35;

    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;

    return S_OK;
}

void CRun_Valkyrie_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Valkyrie_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    if (pOwner->Is_Air())
        return STATE_JUMPFALL_VALKYRIE_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        _uint* pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_VALKYRIE_R, (void*)pInt);
        return STATE_END;

    }

    

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Valkyrie_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Valkyrie_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    

	return __super::Check_Condition(pOwner, pAnimator);
   
}
