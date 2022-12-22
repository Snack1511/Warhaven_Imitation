#include "stdafx.h"
#include "CState_Combat_Attack_Archer_Aiming_Poison.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CAnimWeapon.h"


CState_Combat_Attack_Archer_Aiming_Poison::CState_Combat_Attack_Archer_Aiming_Poison()
{
}

CState_Combat_Attack_Archer_Aiming_Poison::~CState_Combat_Attack_Archer_Aiming_Poison()
{

}

CState_Combat_Attack_Archer_Aiming_Poison* CState_Combat_Attack_Archer_Aiming_Poison::Create()
{
    CState_Combat_Attack_Archer_Aiming_Poison* pInstance = new CState_Combat_Attack_Archer_Aiming_Poison();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Archer_Aiming_Poison");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Archer_Aiming_Poison::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_AIMING_POISION_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.03f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 0;
    
    m_iAINextState = AI_STATE_COMBAT_ATTACK_SHOOT_POISION_ARCHER;

    return __super::Initialize();
}

void CState_Combat_Attack_Archer_Aiming_Poison::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter_Aiming(pOwner, pAnimator, ePrevType);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Archer_Aiming_Poison::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Archer_Aiming_Poison::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit_Aiming(pOwner, pAnimator);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Archer_Aiming_Poison::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Archer_Aiming_Poison::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	 __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
