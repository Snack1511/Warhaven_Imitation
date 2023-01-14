#include "stdafx.h"
#include "CState_Combat_Attack_Default_Lancer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CState_Combat_Attack_Default_Lancer::CState_Combat_Attack_Default_Lancer()
{
}

CState_Combat_Attack_Default_Lancer::~CState_Combat_Attack_Default_Lancer()
{
}

CState_Combat_Attack_Default_Lancer* CState_Combat_Attack_Default_Lancer::Create()
{
    CState_Combat_Attack_Default_Lancer* pInstance = new CState_Combat_Attack_Default_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Default_Lancer::Initialize()
{
    m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
    m_tHitInfo.fKnockBackPower = 3.f;
    m_tHitInfo.fJumpPower = 0.f;


    m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_ATTACKSWING_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fDamagePumping = 2.5f;

    m_fMyAccel = 30.f;
    m_fMyMaxLerp = 0.4f;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 130;
   
    m_iStopIndex = 55;

	Add_KeyFrame(46, 1);
	Add_KeyFrame(m_iStopIndex, 2);
	Add_KeyFrame(30, 999);
	Add_KeyFrame(35, 998);

	m_eBounceState = AI_STATE_COMMON_BOUNCE_LANCER;
    m_iDirectionRand = STATE_DIRECTION_N;

    return __super::Initialize();
}

void CState_Combat_Attack_Default_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    pAnimator->Set_CurAnimIndex(ANIM_BASE_R, 5, ANIM_DIVIDE::eBODYLOWER);
    pAnimator->Get_CurAnimFrame();
    pAnimator->Set_AnimSpeed(ANIM_BASE_R, 5, m_fAnimSpeed);
    pAnimator->Set_InterpolationTime(ANIM_BASE_R, 5, m_fInterPolationTime);

    CUnit* pTargetUnit = pOwner->Get_TargetUnit();

    if (pTargetUnit)
    {
        m_vAIRandLook =  pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
        m_vAIRandLook.Normalize();
    }
    else
        m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

    pOwner->TurnOn_TrailEffect(true);
    pOwner->Set_BounceState(m_eBounceState);



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Default_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEAFULT_LANCER;

    Straight_Move(pOwner);


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Default_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->TurnOn_TrailEffect(false);
    pAnimator->Stop_ActionAnim();
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Default_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Default_Lancer::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
