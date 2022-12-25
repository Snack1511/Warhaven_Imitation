#include "stdafx.h"
#include "CQanda_Shoot_Sniping.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CQanda_Shoot_Sniping::CQanda_Shoot_Sniping()
{
}

CQanda_Shoot_Sniping::~CQanda_Shoot_Sniping()
{
}

CQanda_Shoot_Sniping* CQanda_Shoot_Sniping::Create()
{
	CQanda_Shoot_Sniping* pInstance = new CQanda_Shoot_Sniping();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CQanda_Shoot_Sniping");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CQanda_Shoot_Sniping::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 4.2f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_SHOOT_SNIPING_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 80;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_fMyAccel = 100.f;
	m_fMyMaxLerp = 0.1f;

	m_vecAdjState.push_back(STATE_IDLE_QANDA);
	m_vecAdjState.push_back(STATE_WALK_QANDA);
	m_vecAdjState.push_back(STATE_RUN_QANDA);
	m_vecAdjState.push_back(STATE_JUMP_QANDA);

	m_vecAdjState.push_back(STATE_GUARD_QANDA);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_QANDA);

	// return __super::Initialize();
	return S_OK;
}

void CQanda_Shoot_Sniping::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	Physics_Setting(pOwner->Get_Status().fRunSpeed, pOwner, false);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CQanda_Shoot_Sniping::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > 50)
		m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

	_uint iDirection = Get_Direction();

	if(iDirection != STATE_DIRECTION_END)
		DoMove(iDirection, pOwner);

	return __super::Tick(pOwner, pAnimator);
}

void CQanda_Shoot_Sniping::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_FlyAttackCollider(false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CQanda_Shoot_Sniping::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CQanda_Shoot_Sniping::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		pOwner->Shake_Camera(pOwner->Get_Status().fCamPower , pOwner->Get_Status().fCamTime);
		m_bAttackTrigger = true;
		pOwner->Enable_FlyAttackCollider(true);
		break;

	case 1:
		m_bAttackTrigger = false;
		m_bKeyInputable = true;
		pOwner->Enable_FlyAttackCollider(false);
		break;



	case 2:
		
		break;

	default:
		break;
	}

}
