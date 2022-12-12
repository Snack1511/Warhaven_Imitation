#include "stdafx.h"
#include "CState_Combat_GuardDash_Fiona.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardDash_Fiona::CState_Combat_GuardDash_Fiona()
{
}

CState_Combat_GuardDash_Fiona::~CState_Combat_GuardDash_Fiona()
{
}

CState_Combat_GuardDash_Fiona* CState_Combat_GuardDash_Fiona::Create()
{
    CState_Combat_GuardDash_Fiona* pInstance = new CState_Combat_GuardDash_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardDash_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardDash_Fiona::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 49;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDDASH_FIONA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.4f;

	m_iStateChangeKeyFrame = 20;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_Combat_GuardDash_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = random(0, 4);

	switch (m_iRand)
	{
	case 0:
		m_iAnimIndex = 48;

		break;

	case 1:

		m_iAnimIndex = 49;

		break;

	case 2:

		m_iAnimIndex = 50;

		break;

	case 3:

		m_iAnimIndex = 51;

		break;

	default:
		break;
	}

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	Physics_Setting_AI(pOwner->Get_Status().fGuardDashSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardDash_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
	{
	
		if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		{
			switch (m_iRand)
			{
			case 0:
			case 1:
				return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R;


			case 2:

				return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L;
			
			case 3:

				return AI_STATE_COMBAT_VERTICALCUT_FIONA;

			case 4:

				if(pOwner->Can_Use(CUnit::SKILL3))
					return AI_STATE_COMBAT_SPINATTACK_FIONA;

				else
					return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R;


			default:
				break;
			}

		}
	}
	else
		return AI_STATE_COMBAT_DEAFULT_FIONA_R;


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardDash_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CState_Combat_GuardDash_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
