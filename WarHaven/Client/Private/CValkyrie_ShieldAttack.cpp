#include "stdafx.h"
#include "CValkyrie_ShieldAttack.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CValkyrie_ShieldAttack::CValkyrie_ShieldAttack()
{
}

CValkyrie_ShieldAttack::~CValkyrie_ShieldAttack()
{
}

CValkyrie_ShieldAttack* CValkyrie_ShieldAttack::Create()
{
	CValkyrie_ShieldAttack* pInstance = new CValkyrie_ShieldAttack();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CValkyrie_ShieldAttack");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CValkyrie_ShieldAttack::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 17;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SHIELDATTACK_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 60;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.f;



	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);

	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

	/*나중에 추가*/
	//m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	//m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_VERTICALATTACK_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);


	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

	Add_KeyFrame(12, 0);
	Add_KeyFrame(39, 1);

	// return __super::Initialize();
	return S_OK;
}

void CValkyrie_ShieldAttack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL2);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CValkyrie_ShieldAttack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_ShieldAttack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CValkyrie_ShieldAttack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Can_Use(CUnit::SKILL2))
		return STATE_END;

	/* VALKYRIE가 SpinAttack 로 오는 조건
	1.  R 공격을 누르면 스킬 사용
	*/
	//if (KEY(R, TAP))
	//{
	//	return m_eStateType;
	//}

	return STATE_END;
}

void CValkyrie_ShieldAttack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:

		m_fMaxSpeed = pOwner->Get_Status().fDashAttackSpeed;

		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

		Physics_Setting(m_fMaxSpeed, pOwner);

		pOwner->Shake_Camera(0.1f, 0.05f);
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 1:
		Physics_Setting(0.f, pOwner);

		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}

}
