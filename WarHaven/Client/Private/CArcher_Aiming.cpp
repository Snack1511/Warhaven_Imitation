#include "stdafx.h"
#include "CArcher_Aiming.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"

#include "CAnimWeapon.h"


CArcher_Aiming::CArcher_Aiming()
{
}

CArcher_Aiming::~CArcher_Aiming()
{
}

CArcher_Aiming* CArcher_Aiming::Create()
{
    CArcher_Aiming* pInstance = new CArcher_Aiming();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Aiming");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Aiming::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_AIMING_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 999;

	//Add_KeyFrame(36, 0);

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;

	//Add_KeyFrame(33, 1);
	//Add_KeyFrame(50, 2);

	//Vertical은 전부 Land로 맞춤
	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;
	
	m_iIdle_Index = 11;
	m_iLandRightIndex = 17;
	m_iLandLeftIndex = 8;
	m_iJumpFallRightIndex = 10;
	m_iJumpFallLeftIndex = 0;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 17;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 18;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 19;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 20;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 21;
	
	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 26;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 27;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 28;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 29;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 30;


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

	m_eWalkState = STATE_ATTACK_AIMING_ARCHER;
	m_eJumpState = STATE_ATTACK_AIMING_ARCHER;
	m_eLandState = STATE_ATTACK_AIMING_ARCHER;
	m_eFallState = STATE_ATTACK_AIMING_ARCHER;
	m_eRunState = STATE_ATTACK_AIMING_ARCHER;
	m_eIdleState = STATE_IDLE_ARCHER_R;
	m_eBounceState = STATE_BOUNCE_ARCHER;

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

void CArcher_Aiming::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	if (ePrevType == m_eStateType)
	{		
		pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKBEGIN, FLT_MAX, FLT_MAX);
		pOwner->Set_AnimWeaponFrame(102);
	}

	if (ePrevType == STATE_ATTACK_BEGIN_ARCHER)
	{
		m_fAnimSpeed = FLT_MIN;
		pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLOOP, FLT_MAX, FLT_MIN);
	}

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CArcher_Aiming::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(LBUTTON, AWAY))
		return STATE_ATTACK_SHOOT_ARCHER;
		
	pOwner->Get_FollowCam()->Start_FOVLerp(XMConvertToRadians(15.f));

	BlendableTick_Loop(pOwner, pAnimator);


	return CState::Tick(pOwner, pAnimator);
}

void CArcher_Aiming::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    //Exit에선 무조건 남겨놔야함

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;

    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CArcher_Aiming::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */
   
    //if (KEY(LBUTTON, HOLD))
    //    return m_eStateType;

    return STATE_END;
}

void CArcher_Aiming::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	//switch (iSequence)
	//{

	//case 1:
	//	

	//	m_bAttackTrigger = true;
	//	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
	//	break;

	//case 2:
	//	m_bAttackTrigger = false;
	//	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
	//	break;

	//default:
	//	break;
	//}

}
