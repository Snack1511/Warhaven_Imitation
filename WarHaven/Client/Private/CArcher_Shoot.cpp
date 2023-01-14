#include "stdafx.h"
#include "CArcher_Shoot.h"

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




CArcher_Shoot::CArcher_Shoot()
{
}

CArcher_Shoot::~CArcher_Shoot()
{
}

CArcher_Shoot* CArcher_Shoot::Create()
{
    CArcher_Shoot* pInstance = new CArcher_Shoot();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Shoot");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Shoot::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_SHOOT_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    return __super::Initialize();
}

void CArcher_Shoot::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);	

    Play_Sound(L"Effect_Arrow_Shoot");
}

STATE_TYPE CArcher_Shoot::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CArcher_Shoot::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CArcher_Shoot::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CArcher_Shoot::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
