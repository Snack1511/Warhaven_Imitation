#include "stdafx.h"
#include "CHit_Groggy_Paladin.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_Groggy_Paladin::CHit_Groggy_Paladin()
{
}

CHit_Groggy_Paladin::~CHit_Groggy_Paladin()
{
}

CHit_Groggy_Paladin* CHit_Groggy_Paladin::Create()
{
    CHit_Groggy_Paladin* pInstance = new CHit_Groggy_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Groggy_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_Groggy_Paladin::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GROGGYHIT_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 105;
    
    m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
    m_vecAdjState.push_back(STATE_RUN_PALADIN_R);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);

    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_Groggy_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SHIELDWALL_LOOP_PALADIN ||
        ePrevType == STATE_SHIELDWALL_HIT_PALADIN)
        m_bAttackTrigger = true;

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Groggy_State(pOwner);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_Groggy_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bAttackTrigger)
        return AI_STATE_COMBAT_SHIELDWALL_HIT_PALADIN;

    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PALADIN_R;

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Groggy_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_Groggy_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Attack 으로 오는 조건
    1.  LBuutton 을 이용해 공격한다.
    */
    //if (KEY(CTRL, NONE))
    //{
    //    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    //    {
    //        return m_eStateType;
    //    }
    //}
   
    return STATE_END;
}
