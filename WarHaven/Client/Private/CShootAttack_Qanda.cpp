#include "stdafx.h"
#include "CShootAttack_Qanda.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_Qanda.h"
#include "CAnimWeapon.h"
#include "CUnit_Archer.h"
CShootAttack_Qanda::CShootAttack_Qanda()
{
}

CShootAttack_Qanda::~CShootAttack_Qanda()
{
}

CShootAttack_Qanda* CShootAttack_Qanda::Create()
{
    CShootAttack_Qanda* pInstance = new CShootAttack_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CShootAttack_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CShootAttack_Qanda::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 3.f;
	m_tHitInfo.iLandKeyFrame = 60;
	m_tHitInfo.bFly = true;

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 40;

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;

	Add_KeyFrame(32, 1);

	//Vertical은 전부 Land로 맞춤
	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;

	m_iIdle_Index = 1;
	m_iLandRightIndex = 7;
	m_iLandLeftIndex = 99;
	m_iJumpFallRightIndex = 0;
	m_iJumpFallLeftIndex = 99;

	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 99;



	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 8;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 9;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 10;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 11;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 22;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 23;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 24;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 12;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 18;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 19;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 22;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 23;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 24;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 25;

	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 99;

	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 3;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 4;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 5;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 6;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 2;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;


	m_eWalkState = STATE_WALK_QANDA;
	m_eJumpState = STATE_JUMP_QANDA;
	m_eLandState = STATE_WALK_QANDA;
	m_eFallState = STATE_JUMPFALL_QANDA;
	m_eRunState = STATE_WALK_QANDA;
	m_eIdleState = STATE_IDLE_QANDA;
	m_eBounceState = STATE_WALK_QANDA;


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

void CShootAttack_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP1)
		m_fDamagePumping = 1.f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP2)
		m_fDamagePumping = 1.2f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP3)
	{
		m_fDamagePumping = 1.5f;
		m_iAnimIndex = 8;
	}

	if (ePrevType == STATE_ATTACK_BEGIN_SNIPING_QANDA ||
		ePrevType == STATE_ATTACK_BEGIN_QANDA)
		m_fDamagePumping = 0.7f;

	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLAUNCH, m_fInterPolationTime, m_fAnimSpeed);

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed * 0.7f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed * 0.7f;

	//static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();
	static_cast<CUnit_Qanda*>(pOwner)->Shoot_Crow();
	//static_cast<CUnit_Qanda*>(pOwner)->Enable_AnimWeapon(false);

	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);
    
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CShootAttack_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_QANDA;

    return __super::Tick(pOwner, pAnimator);
}

void CShootAttack_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_SkillTrigger().bSkillETrigger = false;
	pOwner->Set_AnimWeaponIndex(0, m_fInterPolationTime, m_fAnimSpeed);

	if (!m_bAttackTrigger)
		static_cast<CUnit_Qanda*>(pOwner)->Create_Crow();

	static_cast<CUnit_Qanda*>(pOwner)->Enable_AnimWeapon(true);

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	
	pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP_END;

	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CShootAttack_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(LBUTTON, AWAY))
        return m_eStateType;

    return STATE_END;
}

void CShootAttack_Qanda::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{

	case 1:
		static_cast<CUnit_Qanda*>(pOwner)->Create_Crow();
		m_bAttackTrigger = true;
		break;

	default:
		break;
	}

}
