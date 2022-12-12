#include "stdafx.h"
#include "CState_Combat_SkillQ_Counter_Fiona.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillQ_Counter_Fiona::CState_Combat_SkillQ_Counter_Fiona()
{
}

CState_Combat_SkillQ_Counter_Fiona::~CState_Combat_SkillQ_Counter_Fiona()
{
}

CState_Combat_SkillQ_Counter_Fiona* CState_Combat_SkillQ_Counter_Fiona::Create()
{
	CState_Combat_SkillQ_Counter_Fiona* pInstance = new CState_Combat_SkillQ_Counter_Fiona();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Counter_Fiona");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_Counter_Fiona::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_COUNTER_FIONA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	 
	m_iStateChangeKeyFrame = 30;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.7f;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;


	Add_KeyFrame(1, 333);


	return S_OK;
}

void CState_Combat_SkillQ_Counter_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Counter_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEAFULT_FIONA_R;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Counter_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

	pOwner->On_Use(CUnit::SKILL3);
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CState_Combat_SkillQ_Counter_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillQ_Counter_Fiona::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 333:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FionaParring", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;
	default:
		break;
	}
}

