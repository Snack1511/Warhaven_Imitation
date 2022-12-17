#include "stdafx.h"
#include "CState_Common_Slide_Loop_AI.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Slide_Loop_AI::CState_Common_Slide_Loop_AI()
{
}

CState_Common_Slide_Loop_AI::~CState_Common_Slide_Loop_AI()
{
}

CState_Common_Slide_Loop_AI* CState_Common_Slide_Loop_AI::Create()
{
    CState_Common_Slide_Loop_AI* pInstance = new CState_Common_Slide_Loop_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Slide_Loop_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Slide_Loop_AI::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 33;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_SLIDELOOP_AI;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 99;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;


    return S_OK;
}

void CState_Common_Slide_Loop_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Slide_Loop_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 슬라이딩이 끝났다는 상호작용 해야함(트리거 작동).
    //if
     

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Slide_Loop_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CState_Common_Slide_Loop_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    
    //if (pAnimator->Is_CurAnimFinished())
    //    return m_eStateType;


    return STATE_END;
}

