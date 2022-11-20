#include "stdafx.h"
#include "CCharge_WarHammer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CCharge_WarHammer::CCharge_WarHammer()
{
}

CCharge_WarHammer::~CCharge_WarHammer()
{
}

HRESULT CCharge_WarHammer::Initialize()
{

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 1.f;
	m_iStateChangeKeyFrame = 100;

	m_iMaxChargeFrame = 100;
	m_eCurrentChargeKey = KEY::V;

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;

	

	/* Setting for Blendable */
	m_iLandRightIndex = 20;
	m_iLandLeftIndex = 10;
	m_iJumpFallRightIndex = 11;
	m_iJumpFallLeftIndex = 1;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.3f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.15f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.15f;


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

void CCharge_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;
	//m_pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed * 0.5f;
	pOwner->Get_Status().fRunSpeed = m_fMaxSpeed;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CCharge_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CCharge_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed * 2.f;
	pOwner->Get_Status().fRunSpeed = m_fMaxSpeed;

	//Exit에선 무조건 남겨놔야함
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CCharge_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER가 Attack 으로 오는 조건
	1. V 차징 을 이용해 기를 모은다..
	*/

	if (CUser::Get_Instance()->Get_LastKey() == KEY::V)
	{
		_float fDot = CUtility_Transform::Get_LookRotateAngle(pOwner->Get_FollowCamLook());
		if (fabs(fDot) > 0.96f)
		{
			if (pAnimator->Get_CurAnimTypeIndex() == ANIM_BASE_L)
			{
				m_eStateType = STATE_CHARGE_WARHAMMER_L;
			}
			else
			{
				m_eStateType = STATE_CHARGE_WARHAMMER_R;
			}

			
			return m_eStateType;
		}
		else
		{
			if (pAnimator->Get_CurAnimTypeIndex() == ANIM_BASE_L)
			{
				m_eStateType = STATE_CHARGE_FRONT_WARHAMMER_L;
			}
			else
			{
				m_eStateType = STATE_CHARGE_FRONT_WARHAMMER_R;
			}
			
		}

		return m_eStateType;
	}


	return STATE_END;
}

void CCharge_WarHammer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
