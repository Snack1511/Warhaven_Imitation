#include "stdafx.h"
#include "CIdle_Priest.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CIdle_Priest::CIdle_Priest()
{
}

CIdle_Priest::~CIdle_Priest()
{
}

CIdle_Priest* CIdle_Priest::Create()
{
    CIdle_Priest* pInstance = new CIdle_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Priest::Initialize()
{


    // 상하체 분리.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    
    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;


    m_vecAdjState.push_back(STATE_WALK_PRIEST);
    m_vecAdjState.push_back(STATE_JUMP_PRIEST);
    m_vecAdjState.push_back(STATE_RUN_PRIEST);


    Init_AttackState_Priest();
    Init_CommonState_Player();

    
    return S_OK;
}

void CIdle_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SPRINT_END_PRIEST)
        m_fInterPolationTime = 0.f;
    
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CIdle_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PRIEST가 Idle로 오는 조건 
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
