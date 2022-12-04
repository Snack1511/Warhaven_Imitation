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
	m_iAnimIndex = 16;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SHIELDATTACK_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 44;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);

	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);

	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);


	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

	Add_KeyFrame(25, 0);
	Add_KeyFrame(32, 1);
	Add_KeyFrame(50, 2);

	// return __super::Initialize();
	return S_OK;
}

void CValkyrie_ShieldAttack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL2);

	pOwner->Get_OwnerPlayer()->Get_Gauge() -= 15.f;

	pOwner->Set_BounceState(STATE_BOUNCE_VALKYRIE_L);

	Physics_Setting(0.f, pOwner, true);

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((150.f / 255.f), (0.f / 255.f), (0.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON_L;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = 25; // 프레임 맞춰놓음

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CValkyrie_ShieldAttack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();

	if(m_bMoveTrigger)
		pOwner->Get_PhysicsCom()->Set_Accel(m_fMyAccel);

	return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_ShieldAttack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::GUARDBREAK_R, false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CValkyrie_ShieldAttack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	/* VALKYRIE가 SpinAttack 로 오는 조건
	1.  R 공격을 누르면 스킬 사용
	*/
	if (KEY(E, TAP) && pOwner->Get_OwnerPlayer()->Get_Gauge() > 15.f)
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CValkyrie_ShieldAttack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:

		m_fMaxSpeed = pOwner->Get_Status().fSprintAttackSpeed;

		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

		Physics_Setting(m_fMaxSpeed, pOwner);

		m_bAttackTrigger = true;

		pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, true);
		break;

	case 1:
		Physics_Setting(pOwner->Get_Status().fRunSpeed, pOwner);

		m_bAttackTrigger = false;
		pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
		break;

	case 2:
		Physics_Setting(0.f, pOwner);

		m_bMoveTrigger = false;
		break;

	default:
		break;
	}

}
