#include "stdafx.h"
#include "CState_Combat_SkillQ_Paladin_Rush_Begin.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillQ_Paladin_Rush_Begin::CState_Combat_SkillQ_Paladin_Rush_Begin()
{
}

CState_Combat_SkillQ_Paladin_Rush_Begin::~CState_Combat_SkillQ_Paladin_Rush_Begin()
{
}

CState_Combat_SkillQ_Paladin_Rush_Begin* CState_Combat_SkillQ_Paladin_Rush_Begin::Create()
{
	CState_Combat_SkillQ_Paladin_Rush_Begin* pInstance = new CState_Combat_SkillQ_Paladin_Rush_Begin();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Paladin_Rush_Begin");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_Paladin_Rush_Begin::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 22;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_RUSH_BEGIN_PALADIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_fMyAccel = 100.f;

	Add_KeyFrame(50, 0);


	return S_OK;
}

void CState_Combat_SkillQ_Paladin_Rush_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fSprintJumpSpeed;


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Paladin_Rush_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_RUSH_LOOP_PALADIN;

	CUnit* pUnit = pOwner->Get_TargetUnit();
	CTransform* pMyTransform = pOwner->Get_Transform();

	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Paladin_Rush_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Combat_SkillQ_Paladin_Rush_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillQ_Paladin_Rush_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Physics_Setting(pOwner->Get_Status().fDashAttackSpeed, pOwner, false);
		Play_Voice(pOwner, L"Voice_Rush", 1.f);
		Play_Sound(L"Effect_ShieldRush_Wind");
		break;

	default:
		break;
	}
}
