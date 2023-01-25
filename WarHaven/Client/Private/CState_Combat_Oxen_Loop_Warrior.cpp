#include "stdafx.h"
#include "CState_Combat_Oxen_Loop_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CState_Combat_Oxen_Loop_Warrior::CState_Combat_Oxen_Loop_Warrior()
{
}

CState_Combat_Oxen_Loop_Warrior::~CState_Combat_Oxen_Loop_Warrior()
{
}

CState_Combat_Oxen_Loop_Warrior* CState_Combat_Oxen_Loop_Warrior::Create()
{
    CState_Combat_Oxen_Loop_Warrior* pInstance = new CState_Combat_Oxen_Loop_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Oxen_Loop_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Oxen_Loop_Warrior::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 25;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_LOOP;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 3.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 2;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_LOOPATTACK);

    m_eBounceState = STATE_BOUNCE_PLAYER_R;

    m_iEndFinishedFrame = 3;

    Add_KeyFrame(47, 1);

    return S_OK;
}

void CState_Combat_Oxen_Loop_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->TurnOn_TrailEffect(true);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Oxen_Loop_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
    {
        ++m_iCurFinishedFrame;

        if (m_iCurFinishedFrame == m_iEndFinishedFrame)
        {
            m_iCurFinishedFrame = 0;
            return STATE_WARRIOR_OXEN_END;
        }
    }

    // 황소 베기와 오른쪽 공격 같은 것들을 넣어줘야함.
    // 기본적으로 오른손 파지를 전제로 하기 떄문에 Base_L 만 바꿔주면됨.


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Oxen_Loop_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CState_Combat_Oxen_Loop_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Oxen 으로 오는 조건
    1.  Q 을 이용해 공격한다.
        */

    return STATE_END;
}

void CState_Combat_Oxen_Loop_Warrior::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {

    case 1:
        Play_Sound(L"Effect_Oxen_Loop_Attack_Warrior", CHANNEL_EFFECTS, 1.f);
        m_bAttackTrigger = true;
        pOwner->Enable_FlyAttackCollider(true);
        Play_Voice(pOwner, L"Voice_Attack", 1.f);
        break;
    default:
        break;
    }


}

