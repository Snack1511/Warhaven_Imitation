#include "stdafx.h"
#include "CValkyrie_Counter.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CValkyrie_Counter::CValkyrie_Counter()
{
}

CValkyrie_Counter::~CValkyrie_Counter()
{
}

CValkyrie_Counter* CValkyrie_Counter::Create()
{
	CValkyrie_Counter* pInstance = new CValkyrie_Counter();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CValkyrie_Counter");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CValkyrie_Counter::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_COUNTER_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)





	m_eWalkState = STATE_WALK_VALKYRIE_R;
	m_eJumpState = STATE_JUMP_VALKYRIE_R;
	m_eLandState = STATE_JUMP_LAND_VALKYRIE_R;
	m_eFallState = STATE_JUMPFALL_VALKYRIE_R;
	m_eRunState = STATE_RUN_VALKYRIE_R;
	m_eIdleState = STATE_IDLE_VALKYRIE_R;
	m_eBounceState = STATE_SHIELDATTACK_VALKYRIE;


	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);

	m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

	m_iStateChangeKeyFrame = 9999;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.f;

	m_eAnimLeftorRight = ANIM_BASE_R;
	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;

	m_iStopIndex = 90;
	m_iAttackEndIndex = 90;


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


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f  * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f * 0.5f;


	/*나중에 추가*/
	//m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	//m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_VERTICALATTACK_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);


	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);


	return __super::Initialize();
}

void CValkyrie_Counter::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL1);

	pOwner->Get_Status().fRunSpeed *= 0.5f;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CValkyrie_Counter::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 나중에 Hit 됬을 때 Change 하도록 변환
	if (pAnimator->Get_CurAnimFrame() > 30)
		return STATE_SPINATTACK_VALKYRIE;

	return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_Counter::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_Status().fRunSpeed *= 2.f;

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CValkyrie_Counter::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Can_Use(CUnit::SKILL1))
		return STATE_END;

	/* VALKYRIE가 SpinAttack 로 오는 조건
	1.  R 공격을 누르면 스킬 사용
	*/
	if (KEY(Q, TAP))
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CValkyrie_Counter::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
