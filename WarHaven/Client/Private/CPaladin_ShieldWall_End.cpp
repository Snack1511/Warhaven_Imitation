#include "stdafx.h"
#include "CPaladin_ShieldWall_End.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"


CPaladin_ShieldWall_End::CPaladin_ShieldWall_End()
{
}

CPaladin_ShieldWall_End::~CPaladin_ShieldWall_End()
{
}

CPaladin_ShieldWall_End* CPaladin_ShieldWall_End::Create()
{
	CPaladin_ShieldWall_End* pInstance = new CPaladin_ShieldWall_End();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_ShieldWall_End");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_ShieldWall_End::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 4.2f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 35;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SHIELDWALL_END_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 20;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
	m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_PALADIN_R);
	m_vecAdjState.push_back(STATE_JUMP_PALADIN_R);


	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

	m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);



	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);


	return S_OK;
}

void CPaladin_ShieldWall_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	Physics_Setting(0.1f, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_ShieldWall_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_ShieldWall_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CPaladin_ShieldWall_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(E, AWAY))
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CPaladin_ShieldWall_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
