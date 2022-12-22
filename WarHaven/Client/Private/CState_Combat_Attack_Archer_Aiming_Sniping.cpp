#include "stdafx.h"
#include "CState_Combat_Attack_Archer_Aiming_Sniping.h"

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


CState_Combat_Attack_Archer_Aiming_Sniping::CState_Combat_Attack_Archer_Aiming_Sniping()
{
}

CState_Combat_Attack_Archer_Aiming_Sniping::~CState_Combat_Attack_Archer_Aiming_Sniping()
{
}

CState_Combat_Attack_Archer_Aiming_Sniping* CState_Combat_Attack_Archer_Aiming_Sniping::Create()
{
    CState_Combat_Attack_Archer_Aiming_Sniping* pInstance = new CState_Combat_Attack_Archer_Aiming_Sniping();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Archer_Aiming_Sniping");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Archer_Aiming_Sniping::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_AIMING_SNIPING_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.03f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 0;
    
    m_iAINextState = AI_STATE_COMBAT_ATTACK_SHOOT_SNIPING_ARCHER;

	m_fDamagePumping = 4.f;

    return __super::Initialize();
}

void CState_Combat_Attack_Archer_Aiming_Sniping::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter_Aiming(pOwner, pAnimator, ePrevType);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
	GAMEINSTANCE->Start_RadialBlur(0.01f);

}

STATE_TYPE CState_Combat_Attack_Archer_Aiming_Sniping::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Archer_Aiming_Sniping::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit_Aiming(pOwner, pAnimator);
	__super::Exit(pOwner, pAnimator);
	GAMEINSTANCE->Stop_RadialBlur();

}

STATE_TYPE CState_Combat_Attack_Archer_Aiming_Sniping::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Archer_Aiming_Sniping::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	 __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
