#include "stdafx.h"
#include "CState_Combat_Attack_Warrior_VerticalCut.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CState_Combat_Attack_Warrior_VerticalCut::CState_Combat_Attack_Warrior_VerticalCut()
{
}

CState_Combat_Attack_Warrior_VerticalCut::~CState_Combat_Attack_Warrior_VerticalCut()
{
}

CState_Combat_Attack_Warrior_VerticalCut* CState_Combat_Attack_Warrior_VerticalCut::Create()
{
    CState_Combat_Attack_Warrior_VerticalCut* pInstance = new CState_Combat_Attack_Warrior_VerticalCut();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Warrior_VerticalCut::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_VERTICALCUT_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 2.5f;
    m_iStateChangeKeyFrame = 70;
    

    m_iStopIndex = 50;

	Add_KeyFrame(33, 1);
	Add_KeyFrame(m_iStopIndex, 2);

	m_eBounceState = AI_STATE_COMMON_BOUNCE_WARRIOR_R;


    return S_OK;
}

void CState_Combat_Attack_Warrior_VerticalCut::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Set_BounceState(m_eBounceState);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
    m_fMaxSpeed = pOwner->Get_Status().fShortDashSpeed;
}

STATE_TYPE CState_Combat_Attack_Warrior_VerticalCut::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Warrior_VerticalCut::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Warrior_VerticalCut::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Warrior_VerticalCut::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
