#include "stdafx.h"
#include "CState_Combat_SkillE_ShieldAttack_Fiona.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillE_ShieldAttack_Fiona::CState_Combat_SkillE_ShieldAttack_Fiona()
{
}

CState_Combat_SkillE_ShieldAttack_Fiona::~CState_Combat_SkillE_ShieldAttack_Fiona()
{
}

CState_Combat_SkillE_ShieldAttack_Fiona* CState_Combat_SkillE_ShieldAttack_Fiona::Create()
{
	CState_Combat_SkillE_ShieldAttack_Fiona* pInstance = new CState_Combat_SkillE_ShieldAttack_Fiona();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillE_ShieldAttack_Fiona");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillE_ShieldAttack_Fiona::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 16;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SHIELDATTACK_FIONA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;

	m_iStateChangeKeyFrame = 44;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	Add_KeyFrame(25, 0);
	Add_KeyFrame(32, 1);
	Add_KeyFrame(50, 2);

	return S_OK;
}

void CState_Combat_SkillE_ShieldAttack_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->CallBack_CollisionEnter += bind(&CState_Combat_SkillE_ShieldAttack_Fiona::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	pOwner->On_Use(CUnit::SKILL2);

	pOwner->Get_OwnerPlayer()->Get_Gauge() -= 15.f;

	Physics_Setting_AI(0.f, pOwner, true);

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

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Shield_Flare_0"),
		pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE_ShieldAttack_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEAFULT_FIONA_R;

	if(m_bMoveTrigger)
		pOwner->Get_PhysicsCom()->Set_Accel(m_fMyAccel);

	if (!m_bAttackTrigger && m_bHit)
		return AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_R;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE_ShieldAttack_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->CallBack_CollisionEnter -= bind(&CState_Combat_SkillE_ShieldAttack_Fiona::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	pOwner->Enable_UnitCollider(CUnit::GUARDBREAK_R, false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_SkillE_ShieldAttack_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}


void CState_Combat_SkillE_ShieldAttack_Fiona::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
	// m_iRand
	if (iOtherColType == COL_REDHITBOX_BODY || iOtherColType == COL_REDHITBOX_HEAD ||
		iOtherColType == COL_BLUEHITBOX_BODY || iOtherColType == COL_BLUEHITBOX_HEAD)
	{
		m_bHit = true;
	}
}

void CState_Combat_SkillE_ShieldAttack_Fiona::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:

		m_fMaxSpeed = pOwner->Get_Status().fSprintAttackSpeed;

		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

		Physics_Setting_AI(m_fMaxSpeed, pOwner);

		m_bAttackTrigger = true;

		pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, true);

		
		break;

	case 1:
		Physics_Setting(pOwner->Get_Status().fRunSpeed, pOwner);

		m_bAttackTrigger = false;
		pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);


		break;

	case 2:
		Physics_Setting_AI(0.f, pOwner);

		m_bMoveTrigger = false;
		break;

	default:
		break;
	}

}
