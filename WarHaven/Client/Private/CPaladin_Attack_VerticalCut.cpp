#include "stdafx.h"
#include "CPaladin_Attack_VerticalCut.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_Attack_VerticalCut::CPaladin_Attack_VerticalCut()
{
}

CPaladin_Attack_VerticalCut::~CPaladin_Attack_VerticalCut()
{
}

CPaladin_Attack_VerticalCut* CPaladin_Attack_VerticalCut::Create()
{
	CPaladin_Attack_VerticalCut* pInstance = new CPaladin_Attack_VerticalCut();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_Attack_VerticalCut");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_Attack_VerticalCut::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_VERTICALCUT_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_vecAdjState.push_back(m_eStateType);

	m_vecAdjState.push_back(STATE_IDLE_PALADIN_L);
	m_vecAdjState.push_back(STATE_WALK_PALADIN_L);
	m_vecAdjState.push_back(STATE_JUMP_PALADIN_L);
	m_vecAdjState.push_back(STATE_RUN_PALADIN_L);



	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);

	//m_vecAdjState.push_back(STATE_GROGGYATTACK_PALADIN);
	//m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_PALADIN);


	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_PALADIN);


	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);


	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.3f;
	m_iStateChangeKeyFrame = 72;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;


	/* Setting for Blendable */

	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iIdle_Index = 14;

	m_iStopIndex = 39;
	m_iAttackEndIndex = 52;

	Add_KeyFrame(m_iStopIndex, 0);
	Add_KeyFrame(48, 1);

	m_iIdle_Index = 3;
	m_iLandRightIndex = 9;
	m_iLandLeftIndex = 9;
	m_iJumpFallRightIndex = 1;
	m_iJumpFallLeftIndex = 1;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.7f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.7f;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 18;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 19;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 22;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 18;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 19;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 22;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 30;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 37;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 30;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 37;


	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 5;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 6;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 8;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 4; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 5;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 8;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 4; // 제자리
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;

	m_eWalkState = STATE_WALK_PALADIN_L;
	m_eJumpState = STATE_JUMP_PALADIN_L;
	m_eLandState = STATE_JUMP_LAND_PALADIN_L;
	m_eFallState = STATE_JUMPFALL_PALADIN_L;
	m_eRunState = STATE_RUNBEGIN_PALADIN_L;
	m_eIdleState = STATE_IDLE_PALADIN_L;
	m_eBounceState = STATE_BOUNCE_PALADIN_R;

	return __super::Initialize();
}

void CPaladin_Attack_VerticalCut::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_PALADIN_R);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_Attack_VerticalCut::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_Attack_VerticalCut::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CPaladin_Attack_VerticalCut::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* PALADIN가 Walk로 오는 조건
	1. 휠위로 올릴 시
	*/
	if (MOUSE_MOVE(MMS_WHEEL) < 0)
		return m_eStateType;

	return STATE_END;
}

void CPaladin_Attack_VerticalCut::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Effect_Swing_Blunt");
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		Play_Voice(pOwner, L"Voice_Attack", 1.f);
		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}

}
