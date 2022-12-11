#include "stdafx.h"
#include "CState_Combat_Oxen_Cancel_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CState_Combat_Oxen_Cancel_Warrior::CState_Combat_Oxen_Cancel_Warrior()
{
}

CState_Combat_Oxen_Cancel_Warrior::~CState_Combat_Oxen_Cancel_Warrior()
{
}

CState_Combat_Oxen_Cancel_Warrior* CState_Combat_Oxen_Cancel_Warrior::Create()
{
    CState_Combat_Oxen_Cancel_Warrior* pInstance = new CState_Combat_Oxen_Cancel_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Oxen_Cancel_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Oxen_Cancel_Warrior::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 22;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_END;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 20;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);

    return S_OK;
}

void CState_Combat_Oxen_Cancel_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->TurnOn_TrailEffect(true);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Oxen_Cancel_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 황소베기 캔슬(루프 3번)
    if(pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PLAYER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Oxen_Cancel_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CState_Combat_Oxen_Cancel_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}



