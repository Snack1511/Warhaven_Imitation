#include "stdafx.h"
#include "CState_Combat_GuardDash_Engineer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardDash_Engineer::CState_Combat_GuardDash_Engineer()
{
}

CState_Combat_GuardDash_Engineer::~CState_Combat_GuardDash_Engineer()
{
}

CState_Combat_GuardDash_Engineer* CState_Combat_GuardDash_Engineer::Create()
{
    CState_Combat_GuardDash_Engineer* pInstance = new CState_Combat_GuardDash_Engineer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardDash_Engineer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardDash_Engineer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 2;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDDASH_ENGINEER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.4f;

	m_iStateChangeKeyFrame = 20;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_Combat_GuardDash_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = random(0, 5);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	Physics_Setting_AI(pOwner->Get_Status().fGuardDashSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardDash_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
	{
	
		if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		{
			switch (m_iRand)
			{
			case 0:

				return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R;

			case 1:

				return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_L;

			case 2:

				return AI_STATE_COMBAT_VERTICALCUT_ENGINEER_L;

			case 3:

				return AI_STATE_COMBAT_VERTICALCUT_ENGINEER_R;

			case 4:

				if(pOwner->Can_Use(CUnit::SKILL2))
					return AI_STATE_COMBAT_GROGGYATTACK_ENGINEER;

				else
					return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R;


			default:
				break;
			}

		}
	}
	else
		return AI_STATE_COMBAT_DEAFULT_ENGINEER_R;


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardDash_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CState_Combat_GuardDash_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
