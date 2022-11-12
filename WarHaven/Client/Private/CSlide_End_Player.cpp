#include "stdafx.h"
#include "CSlide_End_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSlide_End_Player::CSlide_End_Player()
{
}

CSlide_End_Player::~CSlide_End_Player()
{
}

CSlide_End_Player* CSlide_End_Player::Create()
{
    CSlide_End_Player* pInstance = new CSlide_End_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_End_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_End_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 34;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_END_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 19;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_SPRINT_JUMPFALL_PLAYER);


    return S_OK;
}

void CSlide_End_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    if (ePrevType == STATE_SLIDE_LOOP_PLAYER)
        m_fInterPolationTime = 0.f;

    else
        m_fInterPolationTime = 0.1f;


    __super::Enter(pOwner, pAnimator, ePrevType);

}

STATE_TYPE CSlide_End_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CSlide_End_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
  
}

STATE_TYPE CSlide_End_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;
   

    return STATE_END;
}

