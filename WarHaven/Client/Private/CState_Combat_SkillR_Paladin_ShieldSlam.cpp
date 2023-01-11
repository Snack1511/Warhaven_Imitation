#include "stdafx.h"
#include "CState_Combat_SkillR_Paladin_ShieldSlam.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillR_Paladin_ShieldSlam::CState_Combat_SkillR_Paladin_ShieldSlam()
{
}

CState_Combat_SkillR_Paladin_ShieldSlam::~CState_Combat_SkillR_Paladin_ShieldSlam()
{
}

CState_Combat_SkillR_Paladin_ShieldSlam* CState_Combat_SkillR_Paladin_ShieldSlam::Create()
{
	CState_Combat_SkillR_Paladin_ShieldSlam* pInstance = new CState_Combat_SkillR_Paladin_ShieldSlam();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillR_Paladin_ShieldSlam");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillR_Paladin_ShieldSlam::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;
	
	m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 3.f;
	m_tHitInfo.iLandKeyFrame = 20;
	m_tHitInfo.bFly = true;
	
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SHIELDSLAM_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;
	m_iStateChangeKeyFrame = 72;

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;

	Add_KeyFrame(40, 1);
	Add_KeyFrame(52, 2);


	return __super::Initialize();
}

void CState_Combat_SkillR_Paladin_ShieldSlam::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL1);

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
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

STATE_TYPE CState_Combat_SkillR_Paladin_ShieldSlam::Tick(CUnit* pOwner, CAnimator* pAnimator)
{		
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEFAULT_PALADIN_R;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillR_Paladin_ShieldSlam::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
	pOwner->Enable_FlyAttackCollider(false);
}

STATE_TYPE CState_Combat_SkillR_Paladin_ShieldSlam::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillR_Paladin_ShieldSlam::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{
	case 1:
		if (!pOwner->Is_Air())
		{
			pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
			pOwner->Get_PhysicsCom()->Set_SpeedasMax();
			pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.7f;
			pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
		}

		pOwner->Enable_FlyAttackCollider(true);
		break;
	case 2:
		pOwner->Enable_FlyAttackCollider(false);
		Physics_Setting_AI(0.1f, pOwner);
		break;

	default:
		break;
	}

}
