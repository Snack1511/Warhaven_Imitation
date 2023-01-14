#include "stdafx.h"
#include "CValkyrie_Attack_HorizontalDown.h"

#include "CUtility_Transform.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CValkyrie_Attack_HorizontalDown::CValkyrie_Attack_HorizontalDown()
{
}

CValkyrie_Attack_HorizontalDown::~CValkyrie_Attack_HorizontalDown()
{
}

HRESULT CValkyrie_Attack_HorizontalDown::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 50;

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	//m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_VALKYRIE);

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 10.f;

	m_iStopIndex = 33;
	m_iAttackEndIndex = 50;

	Add_KeyFrame(m_iStopIndex, 0);
	Add_KeyFrame(40, 1);

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	m_iIdle_Index = 3;
	m_iLandRightIndex = 9;
	m_iLandLeftIndex = 9;
	m_iJumpFallRightIndex = 1;
	m_iJumpFallLeftIndex = 1;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 18;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 19;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 33;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 35;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 22;


	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 18;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 19;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 33;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 34;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 35;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 22;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 29;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 30;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 31;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 32;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 33;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 34;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 35;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 36;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 29;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 30;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 31;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 32;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 33;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 34;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 35;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 36;


	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 5;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 6;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 8;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;



	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 5;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 8;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;



	return __super::Initialize();
}

void CValkyrie_Attack_HorizontalDown::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Attack(this);

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CValkyrie_Attack_HorizontalDown::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_Attack_HorizontalDown::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CValkyrie_Attack_HorizontalDown::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 아래로 공격 시  
	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	{
		_float fDot = CUtility_Transform::Get_LookRotateAngle(pOwner->Get_FollowCamLook());

		if (fDot < 0.f && fDot > -0.96f)
		{
			return m_eStateType;
		}
	}
    return STATE_END;
}

void CValkyrie_Attack_HorizontalDown::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 0:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		Play_Voice(pOwner, L"Voice_Attack", 1.f, 1);
		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}

