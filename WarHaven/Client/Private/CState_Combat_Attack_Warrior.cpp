#include "stdafx.h"
#include "CState_Combat_Attack_Warrior.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Attack_Warrior::CState_Combat_Attack_Warrior()
{
}

CState_Combat_Attack_Warrior::~CState_Combat_Attack_Warrior()
{
}

HRESULT CState_Combat_Attack_Warrior::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_Combat_Attack_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_Combat_Attack_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Warrior::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 1:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}

