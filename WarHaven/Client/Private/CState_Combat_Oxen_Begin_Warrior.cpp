#include "stdafx.h"
#include "CState_Combat_Oxen_Begin_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"
#include "Model.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"
#include "HIerarchyNode.h"

CState_Combat_Oxen_Begin_Warrior::CState_Combat_Oxen_Begin_Warrior()
{
}

CState_Combat_Oxen_Begin_Warrior::~CState_Combat_Oxen_Begin_Warrior()
{
}

CState_Combat_Oxen_Begin_Warrior* CState_Combat_Oxen_Begin_Warrior::Create()
{
    CState_Combat_Oxen_Begin_Warrior* pInstance = new CState_Combat_Oxen_Begin_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Oxen_Begin_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Oxen_Begin_Warrior::Initialize()
{


    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 24;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_BEGIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.2f;

    return S_OK;
}

void CState_Combat_Oxen_Begin_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->TurnOn_TrailEffect(true);

 
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Oxen_Begin_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    //애니메이션 끝나면 Loop로
    if (pAnimator->Is_CurAnimFinished())
        return STATE_WARRIOR_OXEN_LOOP;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Oxen_Begin_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CState_Combat_Oxen_Begin_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Oxen_Begin 으로 오는 조건
    1.  Q 을 이용해 공격한다.
     */

    if (!pOwner->Can_Use(CUnit::SKILL2))
        return STATE_END;

    if(KEY(Q, TAP))
        return m_eStateType;

    return STATE_END;
}

