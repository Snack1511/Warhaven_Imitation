#include "stdafx.h"
#include "CState_Combat_Attack_VerticalCut_Engineer_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_Attack_VerticalCut_Engineer_L::CState_Combat_Attack_VerticalCut_Engineer_L()
{
}

CState_Combat_Attack_VerticalCut_Engineer_L::~CState_Combat_Attack_VerticalCut_Engineer_L()
{
}

CState_Combat_Attack_VerticalCut_Engineer_L* CState_Combat_Attack_VerticalCut_Engineer_L::Create()
{
	CState_Combat_Attack_VerticalCut_Engineer_L* pInstance = new CState_Combat_Attack_VerticalCut_Engineer_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_VerticalCut_Engineer_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_Attack_VerticalCut_Engineer_L::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_VERTICALCUT_ENGINEER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_eBounceState = AI_STATE_COMMON_BOUNCE_ENGINEER_L;

	m_iAINextState = AI_STATE_COMBAT_DEAFULT_ENGINEER_L;

	return __super::Initialize();
}

void CState_Combat_Attack_VerticalCut_Engineer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_VerticalCut_Engineer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_VerticalCut_Engineer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_VerticalCut_Engineer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Attack_VerticalCut_Engineer_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
