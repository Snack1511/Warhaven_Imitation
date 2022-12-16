#include "stdafx.h"
#include "CState_Combat_SkillE_Paladin_ShieldWall_Hit.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillE_Paladin_ShieldWall_Hit::CState_Combat_SkillE_Paladin_ShieldWall_Hit()
{
}

CState_Combat_SkillE_Paladin_ShieldWall_Hit::~CState_Combat_SkillE_Paladin_ShieldWall_Hit()
{
}

CState_Combat_SkillE_Paladin_ShieldWall_Hit* CState_Combat_SkillE_Paladin_ShieldWall_Hit::Create()
{
	CState_Combat_SkillE_Paladin_ShieldWall_Hit* pInstance = new CState_Combat_SkillE_Paladin_ShieldWall_Hit();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillE_Paladin_ShieldWall_Hit");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillE_Paladin_ShieldWall_Hit::Initialize()
{

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 20;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SHIELDWALL_HIT_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	return S_OK;
}

void CState_Combat_SkillE_Paladin_ShieldWall_Hit::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE_Paladin_ShieldWall_Hit::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_SHIELDWALL_LOOP_PALADIN;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE_Paladin_ShieldWall_Hit::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CState_Combat_SkillE_Paladin_ShieldWall_Hit::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillE_Paladin_ShieldWall_Hit::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
