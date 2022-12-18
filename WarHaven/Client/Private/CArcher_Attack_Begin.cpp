#include "stdafx.h"
#include "CArcher_Attack_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_Archer.h"
#include "CDefaultArrow.h"
#include "CAnimWeapon.h"


CArcher_Attack_Begin::CArcher_Attack_Begin()
{
}

CArcher_Attack_Begin::~CArcher_Attack_Begin()
{
}

CArcher_Attack_Begin* CArcher_Attack_Begin::Create()
{
    CArcher_Attack_Begin* pInstance = new CArcher_Attack_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Attack_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Attack_Begin::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_BEGIN_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 999;
    
	//m_vecAdjState.push_back(STATE_IDLE_ARCHER_L);
	//m_vecAdjState.push_back(STATE_RUN_ARCHER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_L);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_L);
	//m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	//m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);


	//Add_KeyFrame(36, 0);

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;

	Add_KeyFrame(30, 1);
	Add_KeyFrame(90, 2);

	//Vertical은 전부 Land로 맞춤
	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;
	
	m_iIdle_Index = 11;
	m_iLandRightIndex = 17;
	m_iLandLeftIndex = 8;
	m_iJumpFallRightIndex = 10;
	m_iJumpFallLeftIndex = 0;

	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 28;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 29;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 30;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 31;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 32;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 33;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 35;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 38;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 39;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 45;

	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 38;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 39;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 45;

	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 28;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 29;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 30;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 31;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 32;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 33;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 34;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 35;


	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 4;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 5;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 3;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;

	m_eWalkState = STATE_WALK_ARCHER_R;
	m_eJumpState = STATE_JUMP_ARCHER_R;
	m_eLandState = STATE_JUMP_LAND_ARCHER_R;
	m_eFallState = STATE_JUMPFALL_ARCHER_R;
	m_eRunState = STATE_WALK_ARCHER_R;
	m_eIdleState = STATE_IDLE_ARCHER_R;
	m_eBounceState = STATE_WALK_ARCHER_R;

	//m_eWalkState = STATE_WALK_ARCHER_R;
	//m_eJumpState = STATE_JUMP_ARCHER_R;
	//m_eLandState = STATE_JUMP_LAND_ARCHER_R;
	//m_eFallState = STATE_JUMPFALL_ARCHER_R;
	//m_eRunState = STATE_WALK_ARCHER_R;
	//m_eIdleState = STATE_IDLE_ARCHER_R;
	//m_eBounceState = STATE_WALK_ARCHER_R;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.25f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 0.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 0.8f;

    return __super::Initialize();
}

void CArcher_Attack_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	pOwner->Get_Status().fStoreSpeed = pOwner->Get_Status().fRunSpeed;
	pOwner->Get_Status().fBackStepSpeed = pOwner->Get_Status().fWalkSpeed;
	
	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fWalkSpeed * 0.35f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fRunSpeed;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKBEGIN, m_fInterPolationTime, m_fAnimSpeed);

	m_bMoveTrigger = false;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CArcher_Attack_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// pOwner->Set_BounceState(STATE_BOUNCE_ARCHER_R);

	if (m_bMoveTrigger)
		return STATE_ATTACK_AIMING_ARCHER;

	if (KEY(LBUTTON, AWAY))
	{
		if(m_bAttackTrigger)
			return STATE_ATTACK_SHOOT_ARCHER;
		else 
			return STATE_ATTACK_CANCEL_ARCHER;

	}

	pOwner->Get_FollowCam()->Start_FOVLerp(XMConvertToRadians(15.f));

	BlendableTick_Loop(pOwner, pAnimator);

    return CState::Tick(pOwner, pAnimator);
}

void CArcher_Attack_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    //Exit에선 무조건 남겨놔야함
	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eIDLE, m_fInterPolationTime, m_fAnimSpeed);

    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CArcher_Attack_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */
   

	if (!pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
	{
		if (KEY(LBUTTON, TAP))
			return m_eStateType;
	}


    return STATE_END;
}

void CArcher_Attack_Begin::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{

	case 1:
		m_bAttackTrigger = true;
		break;

	case 2:
		m_bMoveTrigger = true;
		break;

	default:
		break;
	}

}
