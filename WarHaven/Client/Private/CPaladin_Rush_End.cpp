#include "stdafx.h"
#include "CPaladin_Rush_End.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_Rush_End::CPaladin_Rush_End()
{
}

CPaladin_Rush_End::~CPaladin_Rush_End()
{
}

CPaladin_Rush_End* CPaladin_Rush_End::Create()
{
	CPaladin_Rush_End* pInstance = new CPaladin_Rush_End();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_Rush_End");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_Rush_End::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 1.f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 35;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 23;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_RUSH_END_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 40;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_PALADIN);

	m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
	m_vecAdjState.push_back(STATE_JUMP_PALADIN_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_PALADIN_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

	m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
	m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);

	Add_KeyFrame(12, 111);

	return S_OK;
}

void CPaladin_Rush_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL3);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	Physics_Setting(2.f, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_Rush_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_Rush_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CPaladin_Rush_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(Q, AWAY))
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CPaladin_Rush_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 111:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;
	default:
		break;
	}
}
