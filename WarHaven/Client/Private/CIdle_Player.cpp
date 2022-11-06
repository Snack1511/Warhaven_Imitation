#include "stdafx.h"
#include "CIdle_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CIdle_Player::CIdle_Player()
{
}

CIdle_Player::~CIdle_Player()
{
}

CIdle_Player* CIdle_Player::Create()
{
    CIdle_Player* pInstance = new CIdle_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Player::Initialize()
{
    m_eAnimType = ANIM_BASE;
    m_iAnimIndex = 0;
    m_eStateType = STATE_IDLE_PLAYER;

    m_fInterPolationTime = 0.1f;


    //m_vecAdjState.push_back(STATE_WALK_PLAYER);


    return S_OK;
}

void CIdle_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */


    __super::Enter(pOwner, pAnimator);
}

void CIdle_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CIdle_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건 
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
