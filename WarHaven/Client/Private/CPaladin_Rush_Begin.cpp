#include "stdafx.h"
#include "CPaladin_Rush_Begin.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_Rush_Begin::CPaladin_Rush_Begin()
{
}

CPaladin_Rush_Begin::~CPaladin_Rush_Begin()
{
}

CPaladin_Rush_Begin* CPaladin_Rush_Begin::Create()
{
	CPaladin_Rush_Begin* pInstance = new CPaladin_Rush_Begin();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_Rush_Begin");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_Rush_Begin::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 22;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_RUSH_BEGIN_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_fMyAccel = 100.f;
	m_iStateChangeKeyFrame = 99;

	Add_KeyFrame(50, 0);


	// return __super::Initialize();
	return S_OK;
}

void CPaladin_Rush_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fSprintJumpSpeed;


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_Rush_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_RUSH_LOOP_PALADIN;

	pOwner->Get_PhysicsCom()->Set_Accel(m_fMyAccel);
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();
	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_Rush_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CPaladin_Rush_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (!pOwner->Can_Use(CUnit::SKILL3))
		return STATE_END;

	if (KEY(Q, HOLD))
		return m_eStateType;


	return STATE_END;
}

void CPaladin_Rush_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Physics_Setting(pOwner->Get_Status().fDashAttackSpeed, pOwner, false);
		break;

	default:
		break;
	}
}
