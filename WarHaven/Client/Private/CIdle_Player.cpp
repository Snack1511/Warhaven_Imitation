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
    // 상하체 분리.
    // 


    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    
    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    //m_vecAdjState.push_back(STATE_JUMP);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CIdle_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */

  


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CIdle_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
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
