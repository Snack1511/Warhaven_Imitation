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

	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 20;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SHIELDWALL_HIT_PALADIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

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
