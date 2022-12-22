#include "stdafx.h"
#include "CState_Combat_Attack_Archer_Shoot.h"

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

#include "CUnit_Archer.h"




CState_Combat_Attack_Archer_Shoot::CState_Combat_Attack_Archer_Shoot()
{
}

CState_Combat_Attack_Archer_Shoot::~CState_Combat_Attack_Archer_Shoot()
{
}

CState_Combat_Attack_Archer_Shoot* CState_Combat_Attack_Archer_Shoot::Create()
{
    CState_Combat_Attack_Archer_Shoot* pInstance = new CState_Combat_Attack_Archer_Shoot();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Archer_Shoot");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Archer_Shoot::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_SHOOT_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    return __super::Initialize();
}

void CState_Combat_Attack_Archer_Shoot::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);	
}

STATE_TYPE CState_Combat_Attack_Archer_Shoot::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Archer_Shoot::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Archer_Shoot::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Attack_Archer_Shoot::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
