#include "stdafx.h"
#include "CState_Combat_Attack_Default_Shoot_Qanda.h"

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
#include "CProjectile.h"

#include "CUnit_Qanda.h"




CState_Combat_Attack_Default_Shoot_Qanda::CState_Combat_Attack_Default_Shoot_Qanda()
{
}

CState_Combat_Attack_Default_Shoot_Qanda::~CState_Combat_Attack_Default_Shoot_Qanda()
{
}

CState_Combat_Attack_Default_Shoot_Qanda* CState_Combat_Attack_Default_Shoot_Qanda::Create()
{
    CState_Combat_Attack_Default_Shoot_Qanda* pInstance = new CState_Combat_Attack_Default_Shoot_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Default_Shoot_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Default_Shoot_Qanda::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 7;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_SHOOT_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    return __super::Initialize();
}

void CState_Combat_Attack_Default_Shoot_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);	


    Play_Sound(L"Effect_Attack_Qanda");
    Play_Voice(pOwner, L"Voice_Attack", 1.f);

    static_cast<CUnit_Qanda*>(pOwner)->Turn_ChargeEffect(false);
}

STATE_TYPE CState_Combat_Attack_Default_Shoot_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Default_Shoot_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Default_Shoot_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Attack_Default_Shoot_Qanda::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
