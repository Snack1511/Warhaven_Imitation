#include "stdafx.h"
#include "CState_Combat_SkillE_Paladin_ShieldWall_End.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"


CState_Combat_SkillE_Paladin_ShieldWall_End::CState_Combat_SkillE_Paladin_ShieldWall_End()
{
}

CState_Combat_SkillE_Paladin_ShieldWall_End::~CState_Combat_SkillE_Paladin_ShieldWall_End()
{
}

CState_Combat_SkillE_Paladin_ShieldWall_End* CState_Combat_SkillE_Paladin_ShieldWall_End::Create()
{
	CState_Combat_SkillE_Paladin_ShieldWall_End* pInstance = new CState_Combat_SkillE_Paladin_ShieldWall_End();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillE_Paladin_ShieldWall_End");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillE_Paladin_ShieldWall_End::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 4.2f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 35;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SHIELDWALL_END_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 20;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_fMyMaxLerp = 1.2f;

	return S_OK;
}

void CState_Combat_SkillE_Paladin_ShieldWall_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	Physics_Setting(0.f, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE_Paladin_ShieldWall_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		return AI_STATE_COMBAT_DEFAULT_PALADIN_R;

	DoMove_AI(pOwner, pAnimator);

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE_Paladin_ShieldWall_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_SkillE_Paladin_ShieldWall_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillE_Paladin_ShieldWall_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
