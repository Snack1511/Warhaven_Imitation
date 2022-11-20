#include "stdafx.h"
#include "CWarrior_Attack_HorizontalMiddle.h"

#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CWarrior_Attack_HorizontalMiddle::CWarrior_Attack_HorizontalMiddle()
{
}

CWarrior_Attack_HorizontalMiddle::~CWarrior_Attack_HorizontalMiddle()
{
}

HRESULT CWarrior_Attack_HorizontalMiddle::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 70;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;


	m_iStopIndex = 30;
	m_iAttackEndIndex = 55;

	Add_KeyFrame(m_iStopIndex, 1);
	Add_KeyFrame(50, 2);

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;


	return __super::Initialize();

}

void CWarrior_Attack_HorizontalMiddle::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_HorizontalMiddle::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	

	return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalMiddle::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarrior_Attack_HorizontalMiddle::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Walk로 오는 조건
	1. 공격 시
	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	{
		_float fDot = CUtility_Transform::Get_LookRotateAngle(pOwner->Get_FollowCamLook());
		if (fabs(fDot) > 0.96f)
		{

			return m_eStateType;
		}
	}

	return STATE_END;
}

void CWarrior_Attack_HorizontalMiddle::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 1:
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

