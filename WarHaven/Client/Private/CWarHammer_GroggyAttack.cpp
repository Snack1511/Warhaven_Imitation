#include "stdafx.h"
#include "CWarHammer_GroggyAttack.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_GroggyAttack::CWarHammer_GroggyAttack()
{
}

CWarHammer_GroggyAttack::~CWarHammer_GroggyAttack()
{
}

CWarHammer_GroggyAttack* CWarHammer_GroggyAttack::Create()
{
	CWarHammer_GroggyAttack* pInstance = new CWarHammer_GroggyAttack();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_GroggyAttack");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_GroggyAttack::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 13;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GROGGYATTACK_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 60;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);



	Add_KeyFrame(30, 0);
	Add_KeyFrame(32, 1);
	Add_KeyFrame(49, 2);




//	return __super::Initialize();
	return S_OK;
}

void CWarHammer_GroggyAttack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL2);

	m_fMaxSpeed = pOwner->Get_Status().fDashAttackSpeed;
	
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_GroggyAttack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_GroggyAttack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GroggyCollider(false);
}

STATE_TYPE CWarHammer_GroggyAttack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER가 Attack 으로 오는 조건
	1.  LBuutton 을 이용해 공격한다.
	*/

	if (!pOwner->Can_Use(CUnit::SKILL2))
		return STATE_END;

	if (KEY(E, TAP))
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CWarHammer_GroggyAttack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Physics_Setting(m_fMaxSpeed, pOwner);
		break;

	case 1:
		pOwner->Enable_GroggyCollider(true);
		break;

	case 2:
		pOwner->Enable_GroggyCollider(false);
		break;


	default:
		break;
	}

	
	

}
