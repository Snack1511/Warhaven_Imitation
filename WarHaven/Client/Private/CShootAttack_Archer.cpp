#include "stdafx.h"
#include "CShootAttack_Archer.h"

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

CShootAttack_Archer::CShootAttack_Archer()
{
}

CShootAttack_Archer::~CShootAttack_Archer()
{
}

CShootAttack_Archer* CShootAttack_Archer::Create()
{
    CShootAttack_Archer* pInstance = new CShootAttack_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CShootAttack_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CShootAttack_Archer::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

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
	m_eLandState = STATE_RUN_ARCHER_R;
	m_eFallState = STATE_JUMPFALL_ARCHER_R;
	m_eRunState = STATE_RUN_ARCHER_R;
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

	m_bSmootMoveLoop = true;

    return __super::Initialize();
}

void CShootAttack_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP1)
		m_fDamagePumping = 1.f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP2)
		m_fDamagePumping = 1.2f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP3)
		m_fDamagePumping = 1.5f;


	if (ePrevType == STATE_ATTACK_BEGIN_POISION_ARCHER ||
		ePrevType == STATE_ATTACK_BEGIN_SNIPING_ARCHER ||
		ePrevType == STATE_ATTACK_BEGIN_ARCHER)
		m_fDamagePumping = 0.7f;

	_bool bBounce = false;

	if (ePrevType == STATE_ATTACK_BEGIN_POISION_ARCHER ||
		ePrevType == STATE_ATTACK_BEGIN_SNIPING_ARCHER ||
		ePrevType == STATE_ATTACK_BEGIN_ARCHER)
	{
		bBounce = true;
		m_fInterPolationTime = 0.f;
	}

	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLAUNCH, m_fInterPolationTime, m_fAnimSpeed);

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed * 0.7f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed * 0.7f;

	static_cast<CUnit_Archer*>(pOwner)->Shoot_Arrow();

	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);
    
	if (bBounce)
	{
		pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex, ANIM_DIVIDE::eBODYLOWER);
		pAnimator->Set_CurFrame(pOwner->Get_PreAnimIndex());
	}

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CShootAttack_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CShootAttack_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_SkillTrigger().bSkillETrigger = false;
	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eIDLE, m_fInterPolationTime, m_fAnimSpeed);

	if (!m_bAttackTrigger)
		static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	
	pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP_END;

	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CShootAttack_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(LBUTTON, AWAY))
        return m_eStateType;

    return STATE_END;
}

void CShootAttack_Archer::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
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
