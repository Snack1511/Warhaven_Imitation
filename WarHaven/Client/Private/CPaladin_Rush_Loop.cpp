#include "stdafx.h"
#include "CPaladin_Rush_Loop.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_Rush_Loop::CPaladin_Rush_Loop()
{
}

CPaladin_Rush_Loop::~CPaladin_Rush_Loop()
{
}

CPaladin_Rush_Loop* CPaladin_Rush_Loop::Create()
{
	CPaladin_Rush_Loop* pInstance = new CPaladin_Rush_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_Rush_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_Rush_Loop::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 4.2f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 35;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 24;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_RUSH_LOOP_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 1.8f;

	m_vecAdjState.push_back(STATE_RUSH_END_PALADIN);

	return S_OK;
}

void CPaladin_Rush_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
	pOwner->Enable_GroggyCollider(true);

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

	Physics_Setting(m_fMaxSpeed, pOwner);


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_Rush_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fTimeAcc += fDT(0);

	if (m_fTimeAcc > 5.f)
		return STATE_RUSH_END_PALADIN;

	DoMove(STATE_DIRECTION_N, pOwner);

	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();





	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_Rush_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
	pOwner->Enable_GroggyCollider(false);
}

STATE_TYPE CPaladin_Rush_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}
