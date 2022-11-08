#include "stdafx.h"
#include "CIdle_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CIdle_Warrior::CIdle_Warrior()
{
}

CIdle_Warrior::~CIdle_Warrior()
{
}

CIdle_Warrior* CIdle_Warrior::Create()
{
    CIdle_Warrior* pInstance = new CIdle_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CIdle_Warrior::Initialize()
{
    // 상하체 분리.
   
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 41;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
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

void CIdle_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CIdle_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CIdle_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CIdle_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
