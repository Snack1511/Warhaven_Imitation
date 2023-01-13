#include "stdafx.h"
#include "CState_Common_GuardHit_Archer.h"

#include "GameInstance.h"
#include "Physics.h"
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

#include "CUtility_Transform.h"

CState_Common_GuardHit_Archer::CState_Common_GuardHit_Archer()
{
}

CState_Common_GuardHit_Archer::~CState_Common_GuardHit_Archer()
{
}

CState_Common_GuardHit_Archer* CState_Common_GuardHit_Archer::Create()
{
    CState_Common_GuardHit_Archer* pInstance = new CState_Common_GuardHit_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_GuardHit_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_GuardHit_Archer::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // 애니메이션의 메쉬타입
    m_iAnimIndex = m_iGuardIndex[0];                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_GUARDHIT_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_Common_GuardHit_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    /* 날 때린놈의 hit info를 받았다. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Guard_State(pOwner);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_GuardHit_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Can_Use(CUnit::SKILL1))
        return AI_STATE_COMBAT_ATTACK_SWING_ARCHER;

    if (pAnimator->Is_CurAnimFinished())
    {
        STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
        return eDefaultState;
    }


    // 클로 공격 사용
    //if (pAnimator->Get_CurAnimFrame() > 10)
    //  return AI_STATE_COMBAT_HORIZONTALMIDDLE_ARCHER_R;


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_GuardHit_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_GuardHit_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
