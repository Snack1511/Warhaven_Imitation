#include "stdafx.h"
#include "CPaladin_Attack_ShieldSlam.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_Attack_ShieldSlam::CPaladin_Attack_ShieldSlam()
{
}

CPaladin_Attack_ShieldSlam::~CPaladin_Attack_ShieldSlam()
{
}

CPaladin_Attack_ShieldSlam* CPaladin_Attack_ShieldSlam::Create()
{
	CPaladin_Attack_ShieldSlam* pInstance = new CPaladin_Attack_ShieldSlam();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_Attack_ShieldSlam");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_Attack_ShieldSlam::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;
	
	m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 3.f;
	m_tHitInfo.iLandKeyFrame = 20;
	m_tHitInfo.bFly = true;
	
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SHIELDSLAM_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_PALADIN);

	m_vecAdjState.push_back(STATE_IDLE_PALADIN_L);
	m_vecAdjState.push_back(STATE_WALK_PALADIN_L);
	m_vecAdjState.push_back(STATE_JUMP_PALADIN_L);
	m_vecAdjState.push_back(STATE_RUN_PALADIN_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

	m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;
	m_iStateChangeKeyFrame = 999;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;

	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iIdle_Index = 14;

	m_iStopIndex = 40;
	m_iAttackEndIndex = 52;

	Add_KeyFrame(m_iStopIndex, 1);
	Add_KeyFrame(50, 2);

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

	m_fDamagePumping = 1.2f;

	return __super::Initialize();
}

void CPaladin_Attack_ShieldSlam::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL1);

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON_L;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_Attack_ShieldSlam::Tick(CUnit* pOwner, CAnimator* pAnimator)
{		
	if (pAnimator->Is_CurAnimFinished())
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
			return STATE_RUNBEGIN_PALADIN_L;

		else
			return STATE_IDLE_PALADIN_L;
	}

	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_Attack_ShieldSlam::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
	pOwner->Enable_FlyAttackCollider(false);
}

STATE_TYPE CPaladin_Attack_ShieldSlam::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Can_Use(CUnit::SKILL1))
		return STATE_END;

	if (MOUSE_MOVE(MMS_WHEEL) > 0)
		return m_eStateType;


	return STATE_END;
}

void CPaladin_Attack_ShieldSlam::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{
	case 1:
		pOwner->Enable_FlyAttackCollider(true);
		break;
	case 2:
		pOwner->Enable_FlyAttackCollider(false);
		break;
	default:
		break;
	}

}
