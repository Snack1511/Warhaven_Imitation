#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalUp_Warrior_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_Attack_HorizontalUp_Warrior_R::CState_Combat_Attack_HorizontalUp_Warrior_R()
{
}

CState_Combat_Attack_HorizontalUp_Warrior_R::~CState_Combat_Attack_HorizontalUp_Warrior_R()
{
}

CState_Combat_Attack_HorizontalUp_Warrior_R* CState_Combat_Attack_HorizontalUp_Warrior_R::Create()
{
	CState_Combat_Attack_HorizontalUp_Warrior_R* pInstance = new CState_Combat_Attack_HorizontalUp_Warrior_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_HorizontalUp_Warrior_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_Attack_HorizontalUp_Warrior_R::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
	m_tHitInfo.fKnockBackPower = 2.f;
	m_tHitInfo.fJumpPower = 0.f;

	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 5;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_HORIZONTALUP_WARRIOR_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_eBounceState = AI_STATE_COMMON_BOUNCE_WARRIOR_R;

	return __super::Initialize();
}

void CState_Combat_Attack_HorizontalUp_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalUp_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEFAULT_WARRIOR_R;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalUp_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_HorizontalUp_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalUp_Warrior_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
