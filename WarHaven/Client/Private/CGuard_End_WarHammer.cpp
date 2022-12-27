#include "stdafx.h"
#include "CGuard_End_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_End_WarHammer::CGuard_End_WarHammer()
{
}

CGuard_End_WarHammer::~CGuard_End_WarHammer()
{
}

CGuard_End_WarHammer* CGuard_End_WarHammer::Create()
{
    CGuard_End_WarHammer* pInstance = new CGuard_End_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_End_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_End_WarHammer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_END_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);

	Init_CommonState_Player();

    return S_OK;
}

void CGuard_End_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_End_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_End_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_End_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (KEY(RBUTTON, NONE))
		return m_eStateType;
   
    return STATE_END;
}
