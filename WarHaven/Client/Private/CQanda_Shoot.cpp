#include "stdafx.h"
#include "CQanda_Shoot.h"

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




CQanda_Shoot::CQanda_Shoot()
{
}

CQanda_Shoot::~CQanda_Shoot()
{
}

CQanda_Shoot* CQanda_Shoot::Create()
{
    CQanda_Shoot* pInstance = new CQanda_Shoot();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CQanda_Shoot");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CQanda_Shoot::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 7;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_SHOOT_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    return __super::Initialize();
}

void CQanda_Shoot::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);	

    Play_Sound(L"Effect_Attack_Qanda");
    Play_Voice(pOwner, L"Voice_Attack", 1.f);

    static_cast<CUnit_Qanda*>(pOwner)->Turn_ChargeEffect(false);
}

STATE_TYPE CQanda_Shoot::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CQanda_Shoot::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CQanda_Shoot::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CQanda_Shoot::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
