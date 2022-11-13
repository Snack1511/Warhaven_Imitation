#include "stdafx.h"
#include "CWarrior_Attack_Takedown.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Takedown::CWarrior_Attack_Takedown()
{
}

CWarrior_Attack_Takedown::~CWarrior_Attack_Takedown()
{
}

CWarrior_Attack_Takedown* CWarrior_Attack_Takedown::Create()
{
    CWarrior_Attack_Takedown* pInstance = new CWarrior_Attack_Takedown();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Takedown::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_VERTICAL_CUT;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.3f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);


    return S_OK;
}

void CWarrior_Attack_Takedown::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType);
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
}

STATE_TYPE CWarrior_Attack_Takedown::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pOwner->Is_Weapon_R_Collision())
        return STATE_IDLE_PLAYER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Takedown::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

    //Exit에선 무조건 남겨놔야함
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CWarrior_Attack_Takedown::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */

   
    if (MOUSE_MOVE(MMS_WHEEL) < 0)
        return m_eStateType;

    return STATE_END;
}
