#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalDown_Fiona_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_Attack_HorizontalDown_Fiona_L::CState_Combat_Attack_HorizontalDown_Fiona_L()
{
}

CState_Combat_Attack_HorizontalDown_Fiona_L::~CState_Combat_Attack_HorizontalDown_Fiona_L()
{
}

CState_Combat_Attack_HorizontalDown_Fiona_L* CState_Combat_Attack_HorizontalDown_Fiona_L::Create()
{
	CState_Combat_Attack_HorizontalDown_Fiona_L* pInstance = new CState_Combat_Attack_HorizontalDown_Fiona_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_HorizontalDown_Fiona_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_Attack_HorizontalDown_Fiona_L::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;

	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_eBounceState = AI_STATE_COMBAT_GUARDCANCEL_FIONA;

	return __super::Initialize();
}

void CState_Combat_Attack_HorizontalDown_Fiona_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Fiona_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalDown_Fiona_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Fiona_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalDown_Fiona_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
