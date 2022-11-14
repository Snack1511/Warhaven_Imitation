#include "stdafx.h"
#include "CSlide_Begin_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSlide_Begin_Player::CSlide_Begin_Player()
{
}

CSlide_Begin_Player::~CSlide_Begin_Player()
{
}

CSlide_Begin_Player* CSlide_Begin_Player::Create()
{
    CSlide_Begin_Player* pInstance = new CSlide_Begin_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_Begin_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_Begin_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 32;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_BEGIN_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;


    m_vecAdjState.push_back(STATE_SLIDE_LOOP_PLAYER);


    return S_OK;
}

void CSlide_Begin_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{

    __super::Enter(pOwner, pAnimator, ePrevType);

}

STATE_TYPE CSlide_Begin_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CSlide_Begin_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSlide_Begin_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(J, TAP))
    {
        return m_eStateType;
    }

    return STATE_END;
}

