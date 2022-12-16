#include "stdafx.h"
#include "CPaladin_ShieldWall_Loop.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"


CPaladin_ShieldWall_Loop::CPaladin_ShieldWall_Loop()
{
}

CPaladin_ShieldWall_Loop::~CPaladin_ShieldWall_Loop()
{
}

CPaladin_ShieldWall_Loop* CPaladin_ShieldWall_Loop::Create()
{
	CPaladin_ShieldWall_Loop* pInstance = new CPaladin_ShieldWall_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_ShieldWall_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_ShieldWall_Loop::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 21;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SHIELDWALL_LOOP_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 0;

	m_vecAdjState.push_back(STATE_SHIELDWALL_END_PALADIN);

	return S_OK;
}

void CPaladin_ShieldWall_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;	

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_ShieldWall_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();

	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_ShieldWall_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);

	pOwner->On_Use(CUnit::SKILL2);

}

STATE_TYPE CPaladin_ShieldWall_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

	return STATE_END;
}
