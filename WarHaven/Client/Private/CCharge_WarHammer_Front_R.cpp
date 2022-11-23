#include "stdafx.h"
#include "CCharge_WarHammer_Front_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CCharge_WarHammer_Front_R::CCharge_WarHammer_Front_R()
{
}

CCharge_WarHammer_Front_R::~CCharge_WarHammer_Front_R()
{
}

CCharge_WarHammer_Front_R* CCharge_WarHammer_Front_R::Create()
{
	CCharge_WarHammer_Front_R* pInstance = new CCharge_WarHammer_Front_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CCharge_WarHammer_Front_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CCharge_WarHammer_Front_R::Initialize()
{

	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 21;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CHARGE_FRONT_WARHAMMER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	/* Setting for Blendable */

	m_iChargeChangeKeyFrame = 10;

	m_eAnimLeftorRight = ANIM_BASE_R;
	m_iIdle_Index = 14;

	m_eWalkState = STATE_VERTICALATTACK_WARHAMMER_R;
	m_eJumpState = STATE_VERTICALATTACK_WARHAMMER_R;
	m_eLandState = STATE_VERTICALATTACK_WARHAMMER_R;
	m_eFallState = STATE_VERTICALATTACK_WARHAMMER_R;
	m_eRunState = STATE_VERTICALATTACK_WARHAMMER_R;
	m_eIdleState = STATE_VERTICALATTACK_WARHAMMER_R;
	m_eBounceState = STATE_VERTICALATTACK_WARHAMMER_R;
	m_eChargeAttackState = STATE_CHARGEATTACK_FRONT_WARHAMMER_R;


	return __super::Initialize();
}

void CCharge_WarHammer_Front_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CCharge_WarHammer_Front_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CCharge_WarHammer_Front_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CCharge_WarHammer_Front_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CCharge_WarHammer_Front_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
