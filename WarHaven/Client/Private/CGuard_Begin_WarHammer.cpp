#include "stdafx.h"
#include "CGuard_Begin_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_WarHammer::CGuard_Begin_WarHammer()
{
}

CGuard_Begin_WarHammer::~CGuard_Begin_WarHammer()
{
}

CGuard_Begin_WarHammer* CGuard_Begin_WarHammer::Create()
{
    CGuard_Begin_WarHammer* pInstance = new CGuard_Begin_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_WarHammer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_BEGIN_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(STATE_GUARD_LOOP_WARHAMMER);
	m_vecAdjState.push_back(STATE_GUARD_END_WARHAMMER);


    return S_OK;
}

void CGuard_Begin_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.
	Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
    /* 할거없음 */
}

STATE_TYPE CGuard_Begin_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */



	if (KEY(RBUTTON, TAP))
		return m_eStateType;
   
    return STATE_END;
}
