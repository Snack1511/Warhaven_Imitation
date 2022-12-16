#include "stdafx.h"
#include "CState_Combat_GuardDash_Warrior.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardDash_Warrior::CState_Combat_GuardDash_Warrior()
{
}

CState_Combat_GuardDash_Warrior::~CState_Combat_GuardDash_Warrior()
{
}

CState_Combat_GuardDash_Warrior* CState_Combat_GuardDash_Warrior::Create()
{
    CState_Combat_GuardDash_Warrior* pInstance = new CState_Combat_GuardDash_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardDash_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardDash_Warrior::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDDASH_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.4f;

	m_iStateChangeKeyFrame = 20;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 2;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 3;

    return S_OK;
}

void CState_Combat_GuardDash_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_bRight = false;

	m_iRand = random(0, 1);

	m_fAIMyLength = 2.5f;

	if (m_iRand == 0)
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_W];
		m_bRight = false;
	}

	else if (m_iRand == 1)
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_E];
		m_bRight = true;
	}

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	Physics_Setting_Right_AI(pOwner->Get_Status().fGuardDashSpeed, pOwner, true, m_bRight);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardDash_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


	if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
	{
	
		if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		{
			if (m_bRight)
				return AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R;
			else
				return AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L;
		}
	}
	else
		return AI_STATE_COMBAT_DEFAULT_WARRIOR_R;

	Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardDash_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CState_Combat_GuardDash_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
