#include "stdafx.h"
#include "CSprint_Loop.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Loop::CSprint_Loop()
{
}

CSprint_Loop::~CSprint_Loop()
{
}

CSprint_Loop* CSprint_Loop::Create()
{
    CSprint_Loop* pInstance = new CSprint_Loop();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Loop");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Loop::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 60;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_LOOP_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_SPRINT_END_PLAYER);
    m_vecAdjState.push_back(STATE_SPRINT_JUMP_PLAYER);
	m_vecAdjState.push_back(STATE_SPRINTATTACK_BEGIN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CSprint_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    if (ePrevType == STATE_SPRINT_BEGIN_PLAYER)
        m_fInterPolationTime = 0.f;

    else
        m_fInterPolationTime = 0.1f;

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CSprint_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSprint_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 움직인다.
    */
   

    if (pAnimator->Is_CurAnimFinished() && KEY(LSHIFT, HOLD))
        return m_eStateType;

   
    return STATE_END;
}
