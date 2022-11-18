#include "stdafx.h"
#include "CWarHammer_ChargeAttack_Front_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarHammer_ChargeAttack_Front_L::CWarHammer_ChargeAttack_Front_L()
{
}

CWarHammer_ChargeAttack_Front_L::~CWarHammer_ChargeAttack_Front_L()
{
}

CWarHammer_ChargeAttack_Front_L* CWarHammer_ChargeAttack_Front_L::Create()
{
    CWarHammer_ChargeAttack_Front_L* pInstance = new CWarHammer_ChargeAttack_Front_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarHammer_ChargeAttack_Front_L::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_CHARGEATTACK_FRONT_WARHAMMER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	//Vertical은 전부 Land로 맞춤
	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iIdle_Index = 4;


	m_eWalkState = STATE_WALK_WARHAMMER_L;
	m_eJumpState = STATE_JUMP_WARHAMMER_L;
	m_eLandState = STATE_JUMP_LAND_WARHAMMER_L;
	m_eFallState = STATE_JUMPFALL_WARHAMMER_L;
	m_eRunState = STATE_RUN_WARHAMMER_L;
	m_eIdleState = STATE_IDLE_WARHAMMER_L;
	m_eBounceState = STATE_BOUNCE_WARHAMMER_R;


    return __super::Initialize();
}

void CWarHammer_ChargeAttack_Front_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_ChargeAttack_Front_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_ChargeAttack_Front_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

    //Exit에선 무조건 남겨놔야함
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarHammer_ChargeAttack_Front_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
  
	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarHammer_ChargeAttack_Front_L::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
