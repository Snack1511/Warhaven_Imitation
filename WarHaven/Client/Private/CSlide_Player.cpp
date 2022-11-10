#include "stdafx.h"
#include "CSlide_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSlide_Player::CSlide_Player()
{
}

CSlide_Player::~CSlide_Player()
{
}

CSlide_Player* CSlide_Player::Create()
{
    CSlide_Player* pInstance = new CSlide_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 32;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CSlide_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    // Enter 전에 내가 정하고 싶은 State 추가.


    m_eStateType = STATE_SLIDE_PLAYER;
    m_eAnimType = ANIM_ETC;

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CSlide_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(J, NONE))
    {
        if (m_iAnimIndex != 34)
        {
            m_iAnimIndex = 34;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        }
        
    }
        

    if (m_iAnimIndex == 34 && pAnimator->Is_CurAnimFinished())
    {
        return STATE_WALK_PLAYER;
    }
        

    if (m_iAnimIndex == 32 && pAnimator->Is_CurAnimFinished())
    {

        if (m_iAnimIndex != 33)
        {
            m_iAnimIndex = 33;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        }
    }




    return __super::Tick(pOwner, pAnimator);
}

void CSlide_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSlide_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. CTRL 을 누르지 않은 상태
    2. WASD 를 누른 상태
    */

    // m_eStateType 이 End 에 가지 않으면 Enter 를 호출한다.

    // W 랑 A 를 누르면 왼쪽 앞으로 이동한다.
    // W 랑 D 를 누르면 왼쪽 옆으로 이동한다.

    // 만약 WASD 를 눌렀다면

    if (KEY(CTRL, NONE))
    {
        if (KEY(Q, NONE))
        {
            if (KEY(J, HOLD) || 
                m_iAnimIndex == 34 )
            {
                return m_eStateType;
            }
        }

    }
    

   


    return STATE_END;
}

