#include "stdafx.h"
#include "CWarHammer_ChargeAttack_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_ChargeAttack_L::CWarHammer_ChargeAttack_L()
{
}

CWarHammer_ChargeAttack_L::~CWarHammer_ChargeAttack_L()
{
}

CWarHammer_ChargeAttack_L* CWarHammer_ChargeAttack_L::Create()
{
	CWarHammer_ChargeAttack_L* pInstance = new CWarHammer_ChargeAttack_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_ChargeAttack_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_ChargeAttack_L::Initialize()
{


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 10;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CHARGEATTACK_WARHAMMER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_WARHAMMER);

	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);




	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;


	m_eWalkState = STATE_WALK_WARHAMMER_R;
	m_eJumpState = STATE_JUMP_WARHAMMER_R;
	m_eLandState = STATE_JUMP_LAND_WARHAMMER_R;
	m_eFallState = STATE_JUMPFALL_WARHAMMER_R;
	m_eRunState = STATE_RUNBEGIN_WARHAMMER_R;
	m_eIdleState = STATE_IDLE_WARHAMMER_R;
	m_eBounceState = STATE_BOUNCE_WARHAMMER_L;


	return __super::Initialize();
}

void CWarHammer_ChargeAttack_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_ChargeAttack_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimIndex() >= 100)
		__super::On_KeyFrameEvent(pOwner, pAnimator, m_vecKeyFrameEvent[0], 997);

	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_ChargeAttack_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CWarHammer_ChargeAttack_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarHammer_ChargeAttack_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
