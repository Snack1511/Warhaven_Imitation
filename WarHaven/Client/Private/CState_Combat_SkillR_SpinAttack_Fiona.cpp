#include "stdafx.h"
#include "CState_Combat_SkillR_SpinAttack_Fiona.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillR_SpinAttack_Fiona::CState_Combat_SkillR_SpinAttack_Fiona()
{
}

CState_Combat_SkillR_SpinAttack_Fiona::~CState_Combat_SkillR_SpinAttack_Fiona()
{
}

CState_Combat_SkillR_SpinAttack_Fiona* CState_Combat_SkillR_SpinAttack_Fiona::Create()
{
	CState_Combat_SkillR_SpinAttack_Fiona* pInstance = new CState_Combat_SkillR_SpinAttack_Fiona();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillR_SpinAttack_Fiona");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillR_SpinAttack_Fiona::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 17;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SPINATTACK_FIONA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_iStateChangeKeyFrame = 60;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.f;

	Add_KeyFrame(12, 0);
	Add_KeyFrame(39, 1);
	Add_KeyFrame(38, 222);
	Add_KeyFrame(40, 333);

	return S_OK;
}

void CState_Combat_SkillR_SpinAttack_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->TurnOn_TrailEffect(true);

	if (ePrevType != AI_STATE_COMBAT_GUARDCANCEL_FIONA ||
		ePrevType != AI_STATE_COMMON_GUARDHIT_FIONA ||
		ePrevType != AI_STATE_COMMON_GROGGYHIT_FIONA ||
		ePrevType != AI_STATE_COMMON_FLYHIT_FIONA ||
		ePrevType != AI_STATE_COMMON_HIT_FIONA)
	{
		pOwner->On_Use(CUnit::SKILL1);
		m_fInterPolationTime = 0.1f;
	}
	else
		m_fInterPolationTime = 0.f;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillR_SpinAttack_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEFAULT_FIONA_R;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillR_SpinAttack_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->TurnOn_TrailEffect(false);

	pOwner->Enable_GroggyCollider(false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_SkillR_SpinAttack_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillR_SpinAttack_Fiona::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	

	switch (iSequence)
	{
	case 0:

		m_fMaxSpeed = pOwner->Get_Status().fDashAttackSpeed;

		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

		Physics_Setting_AI(m_fMaxSpeed, pOwner);

		m_bAttackTrigger = true;
		pOwner->Enable_GroggyCollider(true);
		break;

	case 1:
		Physics_Setting_AI(0.f, pOwner);

		m_bAttackTrigger = false;
		pOwner->Enable_GroggyCollider(false);
		break;

	case 222:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;
	case 333:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;

	default:
		break;
	}

}
