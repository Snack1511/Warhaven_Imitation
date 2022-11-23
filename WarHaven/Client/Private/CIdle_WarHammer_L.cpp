#include "stdafx.h"
#include "CIdle_WarHammer_L.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CIdle_WarHammer_L::CIdle_WarHammer_L()
{
}

CIdle_WarHammer_L::~CIdle_WarHammer_L()
{
}

CIdle_WarHammer_L* CIdle_WarHammer_L::Create()
{
    CIdle_WarHammer_L* pInstance = new CIdle_WarHammer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_WarHammer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_WarHammer_L::Initialize()
{
 //   __super::Initialize();

    // 상하체 분리.
    // 


    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_WARHAMMER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    
    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.

    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_WARHAMMER);

    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
//    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);


    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


    return S_OK;
}

void CIdle_WarHammer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SWITCH_R_TO_L)
        m_fInterPolationTime = 0.f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_WarHammer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);

}

void CIdle_WarHammer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CIdle_WarHammer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER가 Idle로 오는 조건
	1. 현재 진행중인 애니메이션이 끝났을 때
	*/

	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE))
	{		
	
		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;
	}

    return STATE_END;
}
