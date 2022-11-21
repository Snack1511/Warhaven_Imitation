#include "stdafx.h"
#include "CGuard_Begin_Player_Warrior_AI.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_Player_Warrior_AI::CGuard_Begin_Player_Warrior_AI()
{
}

CGuard_Begin_Player_Warrior_AI::~CGuard_Begin_Player_Warrior_AI()
{
}

CGuard_Begin_Player_Warrior_AI* CGuard_Begin_Player_Warrior_AI::Create()
{
    CGuard_Begin_Player_Warrior_AI* pInstance = new CGuard_Begin_Player_Warrior_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_Player_Warrior_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_Player_Warrior_AI::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(STATE_GUARD_LOOP_WARRIOR_AI_ENEMY);
	// m_vecAdjState.push_back(STATE_GUARD_END_PLAYER);


    return S_OK;
}

void CGuard_Begin_Player_Warrior_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_Player_Warrior_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.

    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_Player_Warrior_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_Begin_Player_Warrior_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   

    return STATE_END;
}
