#include "stdafx.h"
#include "CPriest_Attack_Sting.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CPriest_Attack_Sting::CPriest_Attack_Sting()
{
}

CPriest_Attack_Sting::~CPriest_Attack_Sting()
{
}

CPriest_Attack_Sting* CPriest_Attack_Sting::Create()
{
	CPriest_Attack_Sting* pInstance = new CPriest_Attack_Sting();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPriest_Attack_Sting");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPriest_Attack_Sting::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;
	//m_tHitInfo.bSting = true;


	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_STING_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.4f;

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_iStateChangeKeyFrame = 70;

	m_vecAdjState.push_back(STATE_IDLE_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_PRIEST);

	Init_AttackState_Priest();
	Init_CommonState_Player();

	m_iStopIndex = 40;
	m_iAttackEndIndex = 50;

	Add_KeyFrame(38, 0);
	Add_KeyFrame(50, 1);


	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 1;
	m_iLandRightIndex = 16;
	m_iLandLeftIndex = 16;
	m_iJumpFallRightIndex = 0;
	m_iJumpFallLeftIndex = 0;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 99;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 17;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 18;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 19;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 20;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 24;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 30;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 37;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 99;



	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 12;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 11; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;




	m_eWalkState = STATE_WALK_PRIEST;
	m_eJumpState = STATE_JUMP_PRIEST;
	m_eLandState = STATE_JUMP_LAND_PRIEST;
	m_eFallState = STATE_JUMPFALL_PRIEST;
	m_eRunState = STATE_RUN_PRIEST;
	m_eIdleState = STATE_IDLE_PRIEST;
	m_eBounceState = STATE_BOUNCE_PRIEST;

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

void CPriest_Attack_Sting::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Set_BounceState(STATE_BOUNCE_PRIEST);

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Voice(pOwner, L"Voice_Attack", 1.f);
}

STATE_TYPE CPriest_Attack_Sting::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CPriest_Attack_Sting::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	/* 할거없음 */
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CPriest_Attack_Sting::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(LBUTTON, TAP))
		return m_eStateType;

	return STATE_END;
}


void CPriest_Attack_Sting::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{

	case 0:
		Play_Sound(L"Effect_Sting_Warrior", CHANNEL_EFFECTS);
		pOwner->Get_Status().fDamageMultiplier = 1.f;
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
