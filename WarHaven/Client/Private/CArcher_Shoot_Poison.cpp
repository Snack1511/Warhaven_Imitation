#include "stdafx.h"
#include "CArcher_Shoot_Poison.h"

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

CArcher_Shoot_Poison::CArcher_Shoot_Poison()
{
}

CArcher_Shoot_Poison::~CArcher_Shoot_Poison()
{
}

CArcher_Shoot_Poison* CArcher_Shoot_Poison::Create()
{
    CArcher_Shoot_Poison* pInstance = new CArcher_Shoot_Poison();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Shoot_Poison");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Shoot_Poison::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_SHOOT_POISION_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    return __super::Initialize();
}

void CArcher_Shoot_Poison::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->On_Use(CUnit::SKILL2);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CArcher_Shoot_Poison::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CArcher_Shoot_Poison::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CArcher_Shoot_Poison::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CArcher_Shoot_Poison::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
