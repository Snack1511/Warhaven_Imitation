#include "stdafx.h"
#include "CState_Combat_SkillQ_AirSpike_Loop.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillQ_AirSpike_Loop::CState_Combat_SkillQ_AirSpike_Loop()
{
}

CState_Combat_SkillQ_AirSpike_Loop::~CState_Combat_SkillQ_AirSpike_Loop()
{
}

CState_Combat_SkillQ_AirSpike_Loop* CState_Combat_SkillQ_AirSpike_Loop::Create()
{
	CState_Combat_SkillQ_AirSpike_Loop* pInstance = new CState_Combat_SkillQ_AirSpike_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_AirSpike_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_AirSpike_Loop::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_AIRSPIKE_LOOP_ENGINEER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;


	return __super::Initialize();
}

void CState_Combat_SkillQ_AirSpike_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fSprintJumpSpeed;
	Physics_Setting(m_fMaxSpeed, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_AirSpike_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		return AI_STATE_COMBAT_AIRSPIKE_END_ENGINEER;


	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_AirSpike_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillQ_AirSpike_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}
