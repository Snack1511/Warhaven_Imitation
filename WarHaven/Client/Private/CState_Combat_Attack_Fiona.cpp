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

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    return S_OK;
}

void CState_Combat_Attack_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = random(0, 7);

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

STATE_TYPE CState_Combat_Attack_Fiona::Enter_RandomState(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
	{
		if (Get_TargetLook_Length(pOwner) > 3.f)
		{
			// pOwner->Get_OwnerPlayer()->Get_Gauge()

			

			switch (m_iRand)
			{
			case 0:

				m_iRand = random(0, 1);

				if (m_iRand == 0)
					return AI_STATE_COMBAT_HORIZONTALUP_FIONA_L;
				else
					return AI_STATE_COMBAT_HORIZONTALUP_FIONA_R;


			case 1:


				m_iRand = random(0, 1);

				if (m_iRand == 0)
					return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L;
				else								 
					return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R;
			
			case 2:

				m_iRand = random(0, 1);

				if (m_iRand == 0)
					return AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_L;
				else
					return AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_R;

			case 3:

				return AI_STATE_COMBAT_VERTICALCUT_FIONA;

			case 4:

				return AI_STATE_COMBAT_GUARDBEGIN_FIONA;

			case 5:

				if (pOwner->Get_OwnerPlayer()->Get_Gauge() > 17.f)
					return AI_STATE_COMBAT_SHIELDATTACK_FIONA;
				
				else
					return AI_STATE_COMBAT_GUARDBEGIN_FIONA;

			case 6:

				if (pOwner->Can_Use(CUnit::SKILL1))
					return AI_STATE_COMBAT_SHIELDATTACK_FIONA;

				else
					return AI_STATE_COMBAT_GUARDBEGIN_FIONA;

			case 7:

				if (pOwner->Can_Use(CUnit::SKILL3))
					return AI_STATE_COMBAT_SHIELDATTACK_FIONA;

				else
					return AI_STATE_COMBAT_GUARDBEGIN_FIONA;


			default:

				return AI_STATE_COMBAT_DEAFULT_FIONA_R;

				break;
			}

		}
		else
			return AI_STATE_COMBAT_DEAFULT_FIONA_R;

	}

	return STATE_END;
}



