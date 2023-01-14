#include "stdafx.h"
#include "CState_Combat_SkillQ_Shoot_Sniping_Qanda.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Qanda.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"
#include "CProjectile.h"
#include "CUI_UnitHUD.h"



CState_Combat_SkillQ_Shoot_Sniping_Qanda::CState_Combat_SkillQ_Shoot_Sniping_Qanda()
{
}

CState_Combat_SkillQ_Shoot_Sniping_Qanda::~CState_Combat_SkillQ_Shoot_Sniping_Qanda()
{
}

CState_Combat_SkillQ_Shoot_Sniping_Qanda* CState_Combat_SkillQ_Shoot_Sniping_Qanda::Create()
{
	CState_Combat_SkillQ_Shoot_Sniping_Qanda* pInstance = new CState_Combat_SkillQ_Shoot_Sniping_Qanda();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Shoot_Sniping_Qanda");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_Shoot_Sniping_Qanda::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 4.2f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SHOOT_SNIPING_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 80;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_fMyAccel = 100.f;
	m_fMyMaxLerp = 0.1f;

	m_fDamagePumping = 7.f;

	Add_KeyFrame(30, 0);
	Add_KeyFrame(50, 1);

	// return __super::Initialize();
	return S_OK;
}

void CState_Combat_SkillQ_Shoot_Sniping_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_SnipingTarget = pOwner->Get_MultipleFrustumObject();

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HenshinParticle", pOwner->Get_Transform()->Get_World(WORLD_POS));


	pOwner->Get_Status().fDamageMultiplier = m_fDamagePumping;

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	Physics_Setting(pOwner->Get_Status().fRunSpeed, pOwner, false);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Shoot_Sniping_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEAFULT_QANDA;

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Shoot_Sniping_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

	//if (!m_bAttackTrigger)
	//{
	//	for (auto& elem : m_Mateors)
	//		static_cast<CProjectile*>(elem)->On_ChangePhase(CProjectile::eChase);
	//}

	if (!m_SnipingTarget.empty())
		m_SnipingTarget.clear();

	if (!m_Mateors.empty())
		m_Mateors.clear();

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_SkillQ_Shoot_Sniping_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CState_Combat_SkillQ_Shoot_Sniping_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		m_bAttackTrigger = true;
		break;
	case 1:
		Make_Meteo(pOwner);
		break;
	default:
		break;
	}

}

void CState_Combat_SkillQ_Shoot_Sniping_Qanda::Make_Meteo(CUnit* pOwner)
{

	for (auto& elem : m_SnipingTarget)
	{
		if (static_cast<CUnit*>(elem)->Is_Valid())
		{
			CGameObject* pProjectile = static_cast<CUnit_Qanda*>(pOwner)->Create_Meteor();
			static_cast<CProjectile*>(pProjectile)->Set_TargetUnit(static_cast<CUnit*>(elem));
			m_Mateors.push_back(pProjectile);
		}
	}
}


