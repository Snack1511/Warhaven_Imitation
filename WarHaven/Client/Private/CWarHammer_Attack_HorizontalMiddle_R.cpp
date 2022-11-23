#include "stdafx.h"
#include "CWarHammer_Attack_HorizontalMiddle_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_Attack_HorizontalMiddle_R::CWarHammer_Attack_HorizontalMiddle_R()
{
}

CWarHammer_Attack_HorizontalMiddle_R::~CWarHammer_Attack_HorizontalMiddle_R()
{
}

CWarHammer_Attack_HorizontalMiddle_R* CWarHammer_Attack_HorizontalMiddle_R::Create()
{
	CWarHammer_Attack_HorizontalMiddle_R* pInstance = new CWarHammer_Attack_HorizontalMiddle_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_Attack_HorizontalMiddle_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_Attack_HorizontalMiddle_R::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 7;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_WARHAMMER);

	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_L);

	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);


	/* Setting for Blendable */

	m_eAnimLeftorRight = ANIM_BASE_R;


	//m_eAnimLeftorRight = ANIM_BASE_R;

	//m_iIdle_Index = 14;
	//m_iLandRightIndex = 20;
	//m_iLandLeftIndex = 10;
	//m_iJumpFallRightIndex = 11;
	//m_iJumpFallLeftIndex = 1;





	m_eWalkState = STATE_WALK_WARHAMMER_L;
	m_eJumpState = STATE_JUMP_WARHAMMER_L;
	m_eLandState = STATE_JUMP_LAND_WARHAMMER_L;
	m_eFallState = STATE_JUMPFALL_WARHAMMER_L;
	m_eRunState = STATE_RUNBEGIN_WARHAMMER_L;
	m_eIdleState = STATE_IDLE_WARHAMMER_L;
	m_eBounceState = STATE_BOUNCE_WARHAMMER_R;


	return __super::Initialize();
}

void CWarHammer_Attack_HorizontalMiddle_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_Attack_HorizontalMiddle_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(LBUTTON, HOLD) && pAnimator->Get_CurAnimFrame() >= 11)
		return STATE_CHARGE_WARHAMMER_R;

	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_Attack_HorizontalMiddle_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	

}

STATE_TYPE CWarHammer_Attack_HorizontalMiddle_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

	/* WARHAMMER가 Attack 으로 오는 조건
	1.  LBuutton 을 이용해 공격한다.
	*/
	//if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	//{
	//	return m_eStateType;
	//}

	//return STATE_END;
}

void CWarHammer_Attack_HorizontalMiddle_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
