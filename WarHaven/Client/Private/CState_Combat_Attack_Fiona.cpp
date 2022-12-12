#include "stdafx.h"
#include "CState_Combat_Attack_Fiona.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Attack_Fiona::CState_Combat_Attack_Fiona()
{
}

CState_Combat_Attack_Fiona::~CState_Combat_Attack_Fiona()
{
}

HRESULT CState_Combat_Attack_Fiona::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 1.f;


    return S_OK;
}

void CState_Combat_Attack_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Combat_Attack_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Fiona::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 1:
		m_bAttackTrigger = true;
		m_fMyMaxLerp = 0.01f;
		pOwner->Set_LookToTarget();
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	case 999:

		if (!pOwner->Is_Air())
		{
			pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
			pOwner->Get_PhysicsCom()->Set_SpeedasMax();
			pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.7f;
			pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
		}

		break;

	case 998:

		Physics_Setting_AI(0.1f, pOwner);

		break;

	default:
		break;
	}
}

