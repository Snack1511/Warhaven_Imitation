#include "stdafx.h"
#include "CState_Combat_SkillQ_Paladin_Rush_End.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillQ_Paladin_Rush_End::CState_Combat_SkillQ_Paladin_Rush_End()
{
}

CState_Combat_SkillQ_Paladin_Rush_End::~CState_Combat_SkillQ_Paladin_Rush_End()
{
}

CState_Combat_SkillQ_Paladin_Rush_End* CState_Combat_SkillQ_Paladin_Rush_End::Create()
{
	CState_Combat_SkillQ_Paladin_Rush_End* pInstance = new CState_Combat_SkillQ_Paladin_Rush_End();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Paladin_Rush_End");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_Paladin_Rush_End::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 1.f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 35;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 23;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_RUSH_END_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 40;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	return S_OK;
}

void CState_Combat_SkillQ_Paladin_Rush_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL3);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	Physics_Setting(2.f, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Paladin_Rush_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		return AI_STATE_COMBAT_DEFAULT_PALADIN_R;

	CUnit* pUnit = pOwner->Get_TargetUnit();

	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);

	CTransform* pMyTransform = pOwner->Get_Transform();
	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Paladin_Rush_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_SkillQ_Paladin_Rush_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillQ_Paladin_Rush_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
}
