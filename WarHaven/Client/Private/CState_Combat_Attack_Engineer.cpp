#include "stdafx.h"
#include "CState_Combat_Attack_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Attack_Engineer::CState_Combat_Attack_Engineer()
{
}

CState_Combat_Attack_Engineer::~CState_Combat_Attack_Engineer()
{
}

HRESULT CState_Combat_Attack_Engineer::Initialize()
{
	__super::Initialize();


    return S_OK;
}

void CState_Combat_Attack_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	//if(ePrevType == AI_STATE_COMBAT_OXEN_LOOPATTACK_WARRIOR)
	//	pOwner->Get_Status().fShortDashSpeed = 0.f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_float4 vLook = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	vLook.Normalize();
	vLook.y = 0.f;

	if (!vLook.Is_Zero())
		pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);
	pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_Status().fShortDashSpeed = 8.f;
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Combat_Attack_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Engineer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 1:

		Play_Sound(L"Effect_Swing_Blunt", CHANNEL_EFFECTS, 1.f);
		m_bAttackTrigger = true;
		m_fMyMaxLerp = 0.01f;
		//pOwner->Set_LookToTarget();
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		Play_Voice(pOwner, L"Voice_Attack", 1.f);

		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	case 999:

		if (!pOwner->Is_Air())
		{
			pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed * 0.5f);
			pOwner->Get_PhysicsCom()->Set_SpeedasMax();
			pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.7f;
			pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
		}

		break;

	default:
		break;
	}
}

