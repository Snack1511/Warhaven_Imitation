#include "stdafx.h"
#include "CState_Catch_Catching_Priest.h"

#include "UsefulHeaders.h"


#include "CSword_Effect.h"
#include "CColorController.h"
#include "CUnit_Priest.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"
#include "CScript_FollowCam.h"

#include "CProjectile.h"
#include "Easing_Utillity.h"
#include "CCannonBall.h"
CState_Catch_Catching_Priest::CState_Catch_Catching_Priest()
{
}

CState_Catch_Catching_Priest::~CState_Catch_Catching_Priest()
{
}

CState_Catch_Catching_Priest* CState_Catch_Catching_Priest::Create()
{
	CState_Catch_Catching_Priest* pInstance = new CState_Catch_Catching_Priest();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Catch_Catching_Priest");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CState_Catch_Catching_Priest::Initialize()
{
	//m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 10;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_CATCH_CANNON_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.f;

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_iStateChangeKeyFrame = 99;

	m_fMyAccel = 20.f;
	m_fMyMaxLerp = 0.1f;

	m_fMaxTime = 0.05f;

	m_iDirectionRand = STATE_DIRECTION_N;

	return S_OK;
}

void CState_Catch_Catching_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(true);

	pOwner->Enable_HitBoxColliders(false);

	//pAnimator->Set_CurAnimIndex(ANIM_BASE_R, 31, ANIM_DIVIDE::eBODYLOWER);
	//pAnimator->Set_AnimSpeed(ANIM_BASE_R, 31, m_fAnimSpeed);
	//pAnimator->Set_InterpolationTime(ANIM_BASE_R, 31, m_fInterPolationTime);

	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	if(pTargetUnit)
		pOwner->Get_Status().fDamageMultiplier = pTargetUnit->Get_CurStateP()->Get_DamagePumping();


	__super::Enter(pOwner, pAnimator, ePrevStateType);
}

STATE_TYPE CState_Catch_Catching_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > 5)
		return AI_STATE_CATCH_CANNON_SHOOT_PRIEST;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Catch_Catching_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_HitBoxColliders(true);

	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(false);

	pOwner->On_Use(CUnit::SKILL3);

	if (pOwner->Get_CatchProjectileObject())
		pOwner->Get_CatchProjectileObject()->Use_Collect(false);

	if (pOwner->Get_CatchedBall())
		DISABLE_GAMEOBJECT(pOwner->Get_CatchedBall());

	pAnimator->Stop_ActionAnim();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
}


STATE_TYPE CState_Catch_Catching_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Catch_Catching_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 1000:


		break;


	default:
		break;
	}
}
