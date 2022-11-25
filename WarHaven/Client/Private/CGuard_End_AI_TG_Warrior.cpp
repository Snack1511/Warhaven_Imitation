#include "stdafx.h"
#include "CGuard_End_AI_TG_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_End_AI_TG_Warrior::CGuard_End_AI_TG_Warrior()
{
}

CGuard_End_AI_TG_Warrior::~CGuard_End_AI_TG_Warrior()
{
}

CGuard_End_AI_TG_Warrior* CGuard_End_AI_TG_Warrior::Create()
{
    CGuard_End_AI_TG_Warrior* pInstance = new CGuard_End_AI_TG_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_End_AI_TG_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_End_AI_TG_Warrior::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_GUARD_END_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

    m_vecAdjState.push_back(AI_STATE_IDLE_WARRIOR_L);
    m_vecAdjState.push_back(AI_STATE_RUN_WARRIOR_L);
    m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_L);

    return S_OK;
}

void CGuard_End_AI_TG_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_End_AI_TG_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_End_AI_TG_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_End_AI_TG_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	//if (KEY(RBUTTON, NONE))
	//	return m_eStateType;
   
    return STATE_END;
}
