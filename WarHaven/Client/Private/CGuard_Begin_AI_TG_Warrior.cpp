#include "stdafx.h"
#include "CGuard_Begin_AI_TG_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_AI_TG_Warrior::CGuard_Begin_AI_TG_Warrior()
{
}

CGuard_Begin_AI_TG_Warrior::~CGuard_Begin_AI_TG_Warrior()
{
}

CGuard_Begin_AI_TG_Warrior* CGuard_Begin_AI_TG_Warrior::Create()
{
    CGuard_Begin_AI_TG_Warrior* pInstance = new CGuard_Begin_AI_TG_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_AI_TG_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_AI_TG_Warrior::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_GUARD_BEGIN_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(AI_STATE_GUARD_LOOP_WARRIOR);
	m_vecAdjState.push_back(AI_STATE_GUARD_END_WARRIOR);


    return S_OK;
}

void CGuard_Begin_AI_TG_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_AI_TG_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.
	//Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_AI_TG_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Begin_AI_TG_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */



	//if (KEY(RBUTTON, HOLD))
	//	return m_eStateType;
   
    return STATE_END;
}
