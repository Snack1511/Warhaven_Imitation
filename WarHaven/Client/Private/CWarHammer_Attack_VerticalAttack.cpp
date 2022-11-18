#include "stdafx.h"
#include "CWarHammer_Attack_VerticalAttack.h"

#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWarHammer_Attack_VerticalAttack::CWarHammer_Attack_VerticalAttack()
{
}

CWarHammer_Attack_VerticalAttack::~CWarHammer_Attack_VerticalAttack()
{
}

HRESULT CWarHammer_Attack_VerticalAttack::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;


	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
	m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);

	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_WARHAMMER);

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);


	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.3f;
	m_iStateChangeKeyFrame = 120;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;

	m_iStopIndex = 60;
	m_iAttackEndIndex = 90;

	Add_KeyFrame(m_iStopIndex, 0);
	Add_KeyFrame(80, 1);

	m_iIdle_Index = 4;
	m_iLandRightIndex = 20;
	m_iLandLeftIndex = 10;
	m_iJumpFallRightIndex = 11;
	m_iJumpFallLeftIndex = 1;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 19;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 20;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 21;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 22;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 33;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 35;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 23;


	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 29;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 30;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 31;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 32;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 44;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 45;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 46;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 33;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 30;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 37;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 40;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 41;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 42;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 43;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 44;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 45;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 46;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 47;


	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 16;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 17;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 18;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 19;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 15; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 8;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 9;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 5; // 제자리
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;


	return __super::Initialize();

}

void CWarHammer_Attack_VerticalAttack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_Attack_VerticalAttack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_Attack_VerticalAttack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarHammer_Attack_VerticalAttack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER가 Walk로 오는 조건
	1. 휠위로 올릴 시 
	*/
	if (MOUSE_MOVE(MMS_WHEEL) > 0)
		return m_eStateType;

	return STATE_END;
}

void CWarHammer_Attack_VerticalAttack::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}

	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

