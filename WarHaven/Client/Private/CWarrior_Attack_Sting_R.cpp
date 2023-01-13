#include "stdafx.h"
#include "CWarrior_Attack_Sting_R.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Sting_R::CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R::~CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R* CWarrior_Attack_Sting_R::Create()
{
    CWarrior_Attack_Sting_R* pInstance = new CWarrior_Attack_Sting_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Sting_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Sting_R::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;
	//m_tHitInfo.bSting = true;



	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 13;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_STING_PLAYER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.4f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 70;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);


	m_iStopIndex = 40;
	m_iAttackEndIndex = 50;

	Add_KeyFrame(38, 0);
	Add_KeyFrame(50, 1);


	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 11;
	m_iLandRightIndex = 17;
	m_iLandLeftIndex = 17;
	m_iJumpFallRightIndex = 10;
	m_iJumpFallLeftIndex = 10;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 26;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 27;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 28;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 29;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 30;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 26;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 27;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 28;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 29;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 30;


	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 39;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 45;
	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 38;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 39;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 42;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 45;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 38;



	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 12;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;


	

	m_eWalkState = STATE_WALK_PLAYER_R;
	m_eJumpState = STATE_JUMP_PLAYER_R;
	m_eLandState = STATE_JUMP_LAND_PLAYER_R;
	m_eFallState = STATE_JUMPFALL_PLAYER_R;
	m_eRunState = STATE_RUN_PLAYER_R;
	m_eIdleState = STATE_IDLE_PLAYER_R;
	m_eBounceState = STATE_BOUNCE_PLAYER_R;

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

void CWarrior_Attack_Sting_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_PLAYER_R);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_Sting_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Sting_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarrior_Attack_Sting_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */

   
    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return m_eStateType;

    return STATE_END;
}


void CWarrior_Attack_Sting_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{

	case 0:
		Play_Sound(L"Effect_Sting_Warrior", CHANNEL_EFFECTS, 1.f);
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
