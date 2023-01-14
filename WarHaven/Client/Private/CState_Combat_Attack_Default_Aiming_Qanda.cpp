#include "stdafx.h"
#include "CState_Combat_Attack_Default_Aiming_Qanda.h"

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
#include "HIerarchyNode.h"
#include "CProjectile.h"
#include "CUnit_QANDA.h"

#include "CRectEffects.h"
#include "CAnimWeapon_Crow.h"

CState_Combat_Attack_Default_Aiming_Qanda::CState_Combat_Attack_Default_Aiming_Qanda()
{
}

CState_Combat_Attack_Default_Aiming_Qanda::~CState_Combat_Attack_Default_Aiming_Qanda()
{
}

CState_Combat_Attack_Default_Aiming_Qanda* CState_Combat_Attack_Default_Aiming_Qanda::Create()
{
    CState_Combat_Attack_Default_Aiming_Qanda* pInstance = new CState_Combat_Attack_Default_Aiming_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Default_Aiming_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Default_Aiming_Qanda::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 6;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_AIMING_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;
    m_fAnimSpeed = 1.5f;
    m_iStateChangeKeyFrame = 999;

    return __super::Initialize();
}

void CState_Combat_Attack_Default_Aiming_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	__super::Enter_Aiming(pOwner, pAnimator, ePrevType, CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_QANDA);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
    static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->ChangeColor_Charge();
    static_cast<CUnit_Qanda*>(pOwner)->Turn_FeatherEffect(false);
    static_cast<CUnit_Qanda*>(pOwner)->Turn_SteamEffect(true);
}

STATE_TYPE CState_Combat_Attack_Default_Aiming_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    STATE_TYPE eShootState = STATE_END;

    switch (m_iRand)
    {
    case 0:

        if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP1)
            eShootState = AI_STATE_COMBAT_SHOOT_QANDA;

        break;

    case 1:

        if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP2)
            eShootState = AI_STATE_COMBAT_SHOOT_QANDA;

        break;

    case 2:

        if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP3)
            eShootState  = AI_STATE_COMBAT_SHOOT_QANDA;

        break;

    default:
        break;
    }

    if (eShootState != STATE_END)
    {
        static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->ChangeColor_Shoot();
        m_bShoot = true;
        return eShootState;
    }
	
		
	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Default_Aiming_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{	
	__super::Exit_Aiming(pOwner, pAnimator);
	__super::Exit(pOwner, pAnimator);

    if(!m_bShoot)
        static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->ChangeColor_End();


    if (!m_CrowSteam.empty())
    {
        for (auto& elem : m_CrowSteam)
        {
            static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
        }
        m_CrowSteam.clear();
    }
}

STATE_TYPE CState_Combat_Attack_Default_Aiming_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Default_Aiming_Qanda::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
