#include "stdafx.h"
#include "CState_Combat_SkillQ_Aiming_Sniping_Qanda.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"

#include "CUnit_Qanda.h"
#include "CProjectile.h"

CState_Combat_SkillQ_Aiming_Sniping_Qanda::CState_Combat_SkillQ_Aiming_Sniping_Qanda()
{
}

CState_Combat_SkillQ_Aiming_Sniping_Qanda::~CState_Combat_SkillQ_Aiming_Sniping_Qanda()
{
}

CState_Combat_SkillQ_Aiming_Sniping_Qanda* CState_Combat_SkillQ_Aiming_Sniping_Qanda::Create()
{
	CState_Combat_SkillQ_Aiming_Sniping_Qanda* pInstance = new CState_Combat_SkillQ_Aiming_Sniping_Qanda();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Aiming_Sniping_Qanda");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_Aiming_Sniping_Qanda::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 2;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_AIMING_SNIPING_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.f;


	return S_OK;
}

void CState_Combat_SkillQ_Aiming_Sniping_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fRunBeginSpeed;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Aiming_Sniping_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_SHOOT_SNIPING_QANDA;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Aiming_Sniping_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Combat_SkillQ_Aiming_Sniping_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}




