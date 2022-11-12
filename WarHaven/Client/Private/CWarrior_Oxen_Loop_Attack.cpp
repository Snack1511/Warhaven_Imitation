#include "stdafx.h"
#include "CWarrior_Oxen_Loop_Attack.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen_Loop_Attack::CWarrior_Oxen_Loop_Attack()
{
}

CWarrior_Oxen_Loop_Attack::~CWarrior_Oxen_Loop_Attack()
{
}

CWarrior_Oxen_Loop_Attack* CWarrior_Oxen_Loop_Attack::Create()
{
    CWarrior_Oxen_Loop_Attack* pInstance = new CWarrior_Oxen_Loop_Attack();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen_Loop_Attack");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen_Loop_Attack::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 23;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_LOOPATTACK;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 87;

    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

    return S_OK;
}

void CWarrior_Oxen_Loop_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    pOwner->TurnOn_TrailEffect(true);

    if (ePrevType == STATE_WARRIOR_OXEN_LOOPATTACK)
    {
        m_fInterPolationTime = 0.f;
    }
    else
    {
        m_fInterPolationTime = 0.1f;
    }

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWarrior_Oxen_Loop_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PLAYER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen_Loop_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CWarrior_Oxen_Loop_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Oxen 으로 오는 조건
    1.  Q 을 이용해 공격한다.
        */

    if (KEY(Q, NONE))
        return m_eStateType;

    return STATE_END;
}


