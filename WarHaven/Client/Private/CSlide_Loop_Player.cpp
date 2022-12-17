#include "stdafx.h"
#include "CSlide_Loop_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "Physics.h"

CSlide_Loop_Player::CSlide_Loop_Player()
{
}

CSlide_Loop_Player::~CSlide_Loop_Player()
{
}

CSlide_Loop_Player* CSlide_Loop_Player::Create()
{
    CSlide_Loop_Player* pInstance = new CSlide_Loop_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_Loop_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_Loop_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 33;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_LOOP_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    //m_vecAdjState.push_back(STATE_SLIDE_END_PLAYER);


    return S_OK;
}

void CSlide_Loop_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SLIDE_BEGIN_PLAYER)
        m_fInterPolationTime = 0.f;

    else
        m_fInterPolationTime = 0.1f;

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(10.f);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSlide_Loop_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 상호작용 해야함.

    pOwner->Get_PhysicsCom()->Set_Accel(30.f);


    return __super::Tick(pOwner, pAnimator);
}

void CSlide_Loop_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSlide_Loop_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. CTRL 을 누르지 않은 상태
    2. WASD 를 누른 상태
    */
    
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

   


    return STATE_END;
}

