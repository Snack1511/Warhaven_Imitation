#include "stdafx.h"
#include "CWalk_Paladin_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Paladin_R::CWalk_Paladin_R()
{
}

CWalk_Paladin_R::~CWalk_Paladin_R()
{
}

CWalk_Paladin_R* CWalk_Paladin_R::Create()
{
    CWalk_Paladin_R* pInstance = new CWalk_Paladin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Paladin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Paladin_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 41;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PALADIN_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_PALADIN);

    m_vecAdjState.push_back(STATE_JUMP_PALADIN_R);
    m_vecAdjState.push_back(STATE_RUN_PALADIN_R);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_PALADIN_R);



	// 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;
    
    
    
    


                            



    return S_OK;
}

void CWalk_Paladin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */

    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Paladin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PALADIN_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_PALADIN_R;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Paladin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Paladin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN가 Walk로 오는 조건
    1. 
    */

    return __super::Check_Condition(pOwner, pAnimator);
}
