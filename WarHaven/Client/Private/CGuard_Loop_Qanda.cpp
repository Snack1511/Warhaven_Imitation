#include "stdafx.h"
#include "CGuard_Loop_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Loop_Qanda::CGuard_Loop_Qanda()
{
}

CGuard_Loop_Qanda::~CGuard_Loop_Qanda()
{
}

CGuard_Loop_Qanda* CGuard_Loop_Qanda::Create()
{
    CGuard_Loop_Qanda* pInstance = new CGuard_Loop_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Loop_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Loop_Qanda::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 38;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	m_iStateChangeKeyFrame = 53;

	m_vecAdjState.push_back(STATE_IDLE_QANDA);
	m_vecAdjState.push_back(STATE_WALK_QANDA);
	m_vecAdjState.push_back(STATE_RUN_QANDA);

	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_QANDA);
	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_QANDA);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_QANDA);

	m_vecAdjState.push_back(STATE_SHADOWSTEP_QANDA);

	Init_CommonState_Player();

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.f;


    return S_OK;
}

void CGuard_Loop_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Loop_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_QANDA;

	// 가드 상태에서도 움직일 수 있도록 설정한다.
	Follow_MouseLook(pOwner);


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Loop_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_Loop_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (KEY(RBUTTON, TAP))
		return m_eStateType;
   
    return STATE_END;
}
