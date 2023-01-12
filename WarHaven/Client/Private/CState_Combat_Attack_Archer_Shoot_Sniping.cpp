#include "stdafx.h"
#include "CState_Combat_Attack_Archer_Shoot_Sniping.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_Archer.h"
#include "CAnimWeapon.h"


CState_Combat_Attack_Archer_Shoot_Sniping::CState_Combat_Attack_Archer_Shoot_Sniping()
{
}

CState_Combat_Attack_Archer_Shoot_Sniping::~CState_Combat_Attack_Archer_Shoot_Sniping()
{
}

CState_Combat_Attack_Archer_Shoot_Sniping* CState_Combat_Attack_Archer_Shoot_Sniping::Create()
{
    CState_Combat_Attack_Archer_Shoot_Sniping* pInstance = new CState_Combat_Attack_Archer_Shoot_Sniping();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Archer_Shoot_Sniping");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Archer_Shoot_Sniping::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_ATTACK_SHOOT_SNIPING_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fDamagePumping = 4.f;

    return __super::Initialize();
}

void CState_Combat_Attack_Archer_Shoot_Sniping::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->On_Use(CUnit::SKILL3);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Archer_Shoot_Sniping::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Archer_Shoot_Sniping::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Archer_Shoot_Sniping::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Attack_Archer_Shoot_Sniping::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
