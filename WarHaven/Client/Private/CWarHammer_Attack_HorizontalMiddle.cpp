#include "stdafx.h"
#include "CWarHammer_Attack_HorizontalMiddle.h"

#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"

#include "CEffects_Factory.h"

CWarHammer_Attack_HorizontalMiddle::CWarHammer_Attack_HorizontalMiddle()
{
}

CWarHammer_Attack_HorizontalMiddle::~CWarHammer_Attack_HorizontalMiddle()
{
}

HRESULT CWarHammer_Attack_HorizontalMiddle::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
	m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 110;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;


	m_iStopIndex = 58;
	m_iAttackEndIndex = 92;

	Add_KeyFrame(m_iStopIndex, 1);
	Add_KeyFrame(80, 2);

	m_iIdle_Index = 4;
	m_iLandRightIndex = 20;
	m_iLandLeftIndex = 10;
	m_iJumpFallRightIndex = 11;
	m_iJumpFallLeftIndex = 1;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.7f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.7f;


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

void CWarHammer_Attack_HorizontalMiddle::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HammerFlare", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HammerFlare_1"), pOwner,
		pOwner->Get_Transform()->Get_World(WORLD_POS)); //test

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Voice(pOwner, L"Voice_Attack", 1.f);
}

STATE_TYPE CWarHammer_Attack_HorizontalMiddle::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	

	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_Attack_HorizontalMiddle::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarHammer_Attack_HorizontalMiddle::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER가 Walk로 오는 조건
	1. 공격 시
	*/

	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	{
		//_float fDot = CUtility_Transform::Get_LookRotateAngle(pOwner->Get_FollowCamLook());
		//if (fabs(fDot) > 0.96f)
		{

			return m_eStateType;
		}
	}

	return STATE_END;
}

void CWarHammer_Attack_HorizontalMiddle::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 1:
		Play_Sound(L"Effect_Swing_Blunt", CHANNEL_EFFECTS, 1.f);
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}

