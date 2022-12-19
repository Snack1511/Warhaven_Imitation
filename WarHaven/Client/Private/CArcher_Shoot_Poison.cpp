#include "stdafx.h"
#include "CArcher_Shoot_Poison.h"

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
#include "CAnimWeapon.h"

CArcher_Shoot_Poison::CArcher_Shoot_Poison()
{
}

CArcher_Shoot_Poison::~CArcher_Shoot_Poison()
{
}

CArcher_Shoot_Poison* CArcher_Shoot_Poison::Create()
{
    CArcher_Shoot_Poison* pInstance = new CArcher_Shoot_Poison();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Shoot_Poison");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Shoot_Poison::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_SHOOT_POISION_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 0;
    
	m_vecAdjState.push_back(STATE_GUARD_ARCHER);
	m_vecAdjState.push_back(STATE_ATTACK_SWING_ARCHER);

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;

	Add_KeyFrame(32, 1);

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


	m_eWalkState = STATE_WALK_ARCHER_R;
	m_eJumpState = STATE_JUMP_ARCHER_R;
	m_eLandState = STATE_WALK_ARCHER_R;
	m_eFallState = STATE_JUMPFALL_ARCHER_R;
	m_eRunState = STATE_WALK_ARCHER_R;
	m_eIdleState = STATE_IDLE_ARCHER_R;
	m_eBounceState = STATE_WALK_ARCHER_R;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	m_bLandMove = true;

    return __super::Initialize();
}

void CArcher_Shoot_Poison::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL2);

	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLAUNCH, m_fInterPolationTime, m_fAnimSpeed);

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed * 0.7f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed * 0.7f;

	static_cast<CUnit_Archer*>(pOwner)->Shoot_Arrow();

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CArcher_Shoot_Poison::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// pOwner->Set_BounceState(STATE_BOUNCE_ARCHER_R);

	//if (pAnimator->Is_CurAnimFinished())
	//	return STATE_ATTACK_SWAP_ARCHER;

	pOwner->Get_FollowCam()->Start_FOVLerp(XMConvertToRadians(45.f));

    return __super::Tick(pOwner, pAnimator);
}

void CArcher_Shoot_Poison::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_SkillTrigger().bSkillETrigger = false;
	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eIDLE, m_fInterPolationTime, m_fAnimSpeed);

	if (!m_bAttackTrigger)
		static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

	
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CArcher_Shoot_Poison::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(LBUTTON, AWAY))
        return m_eStateType;

    return STATE_END;
}

void CArcher_Shoot_Poison::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{

	case 1:
		static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();
		m_bAttackTrigger = true;
		break;

	default:
		break;
	}

}
