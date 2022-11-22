#include "stdafx.h"
#include "CGuard_Loop_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Loop_Player::CGuard_Loop_Player()
{
}

CGuard_Loop_Player::~CGuard_Loop_Player()
{
}

CGuard_Loop_Player* CGuard_Loop_Player::Create()
{
    CGuard_Loop_Player* pInstance = new CGuard_Loop_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Loop_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Loop_Player::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_LOOP_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.f;

	//m_vecAdjState.push_back(STATE_GUARD_CANCEL_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_END_PLAYER);



    return S_OK;
}

void CGuard_Loop_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Enable_GuardCollider(true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Loop_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.
	Follow_MouseLook(pOwner);


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Loop_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Loop_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (pAnimator->Is_CurAnimFinished())
	{
		if (KEY(RBUTTON, HOLD))
			return m_eStateType;
	}

   
    return STATE_END;
}
