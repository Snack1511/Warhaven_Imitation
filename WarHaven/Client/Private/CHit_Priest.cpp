#include "stdafx.h"
#include "CHit_Priest.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"
#include "CProjectile.h"

CHit_Priest::CHit_Priest()
{
}

CHit_Priest::~CHit_Priest()
{
}

CHit_Priest* CHit_Priest::Create()
{
    CHit_Priest* pInstance = new CHit_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_Priest::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_HIT_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 50;
    
    m_vecAdjState.push_back(STATE_WALK_PRIEST);
    m_vecAdjState.push_back(STATE_RUN_PRIEST);
    m_vecAdjState.push_back(STATE_ATTACK_STING_PRIEST);

    Init_AttackState_Priest();
    Init_CommonState_Player();

    m_fMyAccel = 1.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.1f;

    return __super::Initialize();
}

void CHit_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Hit_State(pOwner);


    if (pOwner->Get_CatchProjectileObject())
    {
        pOwner->Get_CatchProjectileObject()->On_ShootProjectile();
        pOwner->Catch_ProjectileObject(nullptr);
    }
        

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
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
