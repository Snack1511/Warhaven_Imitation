#include "stdafx.h"
#include "CAI_CWarrior_Attack_HorizontalMiddle.h"

#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CAI_CWarrior_Attack_HorizontalMiddle::CAI_CWarrior_Attack_HorizontalMiddle()
{
}

CAI_CWarrior_Attack_HorizontalMiddle::~CAI_CWarrior_Attack_HorizontalMiddle()
{
}

HRESULT CAI_CWarrior_Attack_HorizontalMiddle::Initialize()
{
	//m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;
	//m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 70;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;


	//m_iStopIndex = 30;
	//m_iAttackEndIndex = 55;

	//Add_KeyFrame(m_iStopIndex, 1);
	//Add_KeyFrame(50, 2);

	//m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	//m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	//m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	//m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	//m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	//m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	//m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	//m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	Add_KeyFrame(30, 0);
	Add_KeyFrame(50, 1);

	return S_OK;
}

void CAI_CWarrior_Attack_HorizontalMiddle::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->CallBack_CollisionEnter += bind(&CAI_CWarrior_Attack_HorizontalMiddle::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);


	// pOwner->On_Attack(this);

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CAI_CWarrior_Attack_HorizontalMiddle::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CAI_CWarrior_Attack_HorizontalMiddle::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->CallBack_CollisionEnter -= bind(&CAI_CWarrior_Attack_HorizontalMiddle::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	//__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CAI_CWarrior_Attack_HorizontalMiddle::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


	return STATE_END;
}

void CAI_CWarrior_Attack_HorizontalMiddle::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{	
	if(iOtherColType == COL_PLAYERGUARD)
		Bounce_State(pOtherObject, iOtherColType, iMyColType, vHitPos);
}


void CAI_CWarrior_Attack_HorizontalMiddle::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

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
}

