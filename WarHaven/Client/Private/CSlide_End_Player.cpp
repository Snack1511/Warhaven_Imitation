#include "stdafx.h"
#include "CSlide_End_Player.h"

#include "UsefulHeaders.h"

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

    m_iStateChangeKeyFrame = 99;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CSlide_End_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSlide_End_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        STATE_TYPE eSprintEndState = pOwner->Get_SprintEndState();
        return eSprintEndState;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CSlide_End_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
  
}

STATE_TYPE CSlide_End_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

