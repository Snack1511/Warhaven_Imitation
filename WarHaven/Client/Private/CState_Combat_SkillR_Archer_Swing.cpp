#include "stdafx.h"
#include "CState_Combat_SkillR_Archer_Swing.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Archer.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"
#include "CProjectile.h"


CState_Combat_SkillR_Archer_Swing::CState_Combat_SkillR_Archer_Swing()
{
}

CState_Combat_SkillR_Archer_Swing::~CState_Combat_SkillR_Archer_Swing()
{
}

CState_Combat_SkillR_Archer_Swing* CState_Combat_SkillR_Archer_Swing::Create()
{
	CState_Combat_SkillR_Archer_Swing* pInstance = new CState_Combat_SkillR_Archer_Swing();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillR_Archer_Swing");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillR_Archer_Swing::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_ATTACK_SWING_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.3f;
	m_iStateChangeKeyFrame = 70;

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;

	Add_KeyFrame(41, 1);
	Add_KeyFrame(48, 2);

	m_eBounceState = AI_STATE_COMMON_BOUNCE_ARCHER;

	return __super::Initialize();
}

void CState_Combat_SkillR_Archer_Swing::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	
	pOwner->Set_BounceState(m_eBounceState);

	pOwner->On_Use(CUnit::SKILL1);
	static_cast<CUnit_Archer*>(pOwner)->Change_ArrowPhase((_uint)CProjectile::eLOOP);

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((255.f / 255.f), (140.f / 255.f), (42.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON_L;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillR_Archer_Swing::Tick(CUnit* pOwner, CAnimator* pAnimator)
{		
	if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		return AI_STATE_COMBAT_DEFAULT_ARCHER_R;

	CUnit* pUnit = pOwner->Get_TargetUnit();
	CTransform* pMyTransform = pOwner->Get_Transform();

	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillR_Archer_Swing::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
	static_cast<CUnit_Archer*>(pOwner)->Change_ArrowPhase((_uint)CProjectile::eSTART);
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
}

STATE_TYPE CState_Combat_SkillR_Archer_Swing::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillR_Archer_Swing::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 1:
		if (!pOwner->Is_Air())
		{
			m_bAttackTrigger = true;
			pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
			pOwner->Get_PhysicsCom()->Set_SpeedasMax();
			pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.7f;
			pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
		}

		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;
	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		Physics_Setting_AI(0.1f, pOwner);
		break;

	default:
		break;
	}

}
