#include "stdafx.h"
#include "CWarHammer_ChargeAttack_Front_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_ChargeAttack_Front_R::CWarHammer_ChargeAttack_Front_R()
{
}

CWarHammer_ChargeAttack_Front_R::~CWarHammer_ChargeAttack_Front_R()
{
}

CWarHammer_ChargeAttack_Front_R* CWarHammer_ChargeAttack_Front_R::Create()
{
	CWarHammer_ChargeAttack_Front_R* pInstance = new CWarHammer_ChargeAttack_Front_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_ChargeAttack_Front_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_ChargeAttack_Front_R::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 5;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CHARGEATTACK_FRONT_WARHAMMER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	/* Setting for Blendable */

	m_eAnimLeftorRight = ANIM_BASE_R;
	m_iIdle_Index = 14;

	m_eWalkState = STATE_WALK_WARHAMMER_L;
	m_eJumpState = STATE_JUMP_WARHAMMER_L;
	m_eLandState = STATE_JUMP_LAND_WARHAMMER_L;
	m_eFallState = STATE_JUMPFALL_WARHAMMER_L;
	m_eRunState = STATE_RUNBEGIN_WARHAMMER_L;
	m_eIdleState = STATE_IDLE_WARHAMMER_L;
	m_eBounceState = STATE_BOUNCE_WARHAMMER_R;

	m_fDamagePumping = 5.6f;

	return __super::Initialize();
}

void CWarHammer_ChargeAttack_Front_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_WARHAMMER_R);


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_ChargeAttack_Front_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_ChargeAttack_Front_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarHammer_ChargeAttack_Front_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarHammer_ChargeAttack_Front_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
