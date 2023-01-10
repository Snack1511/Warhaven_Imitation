#include "stdafx.h"
#include "CQanda_Shoot_Sniping.h"

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



CQanda_Shoot_Sniping::CQanda_Shoot_Sniping()
{
}

CQanda_Shoot_Sniping::~CQanda_Shoot_Sniping()
{
}

CQanda_Shoot_Sniping* CQanda_Shoot_Sniping::Create()
{
	CQanda_Shoot_Sniping* pInstance = new CQanda_Shoot_Sniping();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CQanda_Shoot_Sniping");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CQanda_Shoot_Sniping::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 4.2f;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_SHOOT_SNIPING_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 80;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_fMyAccel = 100.f;
	m_fMyMaxLerp = 0.1f;

	m_vecAdjState.push_back(STATE_IDLE_QANDA);
	m_vecAdjState.push_back(STATE_WALK_QANDA);
	m_vecAdjState.push_back(STATE_RUN_QANDA);
	m_vecAdjState.push_back(STATE_JUMP_QANDA);

	m_vecAdjState.push_back(STATE_GUARD_QANDA);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_QANDA);

	m_fDamagePumping = 7.f;

	Add_KeyFrame(30, 0);
	Add_KeyFrame(50, 1);

	// return __super::Initialize();
	return S_OK;
}

void CQanda_Shoot_Sniping::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_SnipingTarget = pOwner->Get_MultipleFrustumObject();

	/* ColorControl*/
	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 0.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = 45; // 프레임 맞춰놓음

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	pOwner->Get_Status().fDamageMultiplier = m_fDamagePumping;

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	Physics_Setting(pOwner->Get_Status().fRunSpeed, pOwner, false);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CQanda_Shoot_Sniping::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Enable_TargetUI();

	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_QANDA;

	if (pAnimator->Get_CurAnimFrame() > 50)
		m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

	_uint iDirection = Get_Direction();

	if (iDirection != STATE_DIRECTION_END)
		DoMove(iDirection, pOwner);

	return __super::Tick(pOwner, pAnimator);
}

void CQanda_Shoot_Sniping::Exit(CUnit* pOwner, CAnimator* pAnimator)
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

	Disable_TargetUI();

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CQanda_Shoot_Sniping::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CQanda_Shoot_Sniping::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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

void CQanda_Shoot_Sniping::Make_Meteo(CUnit* pOwner)
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HenshinParticle", pOwner->Get_Transform()->Get_World(WORLD_POS));

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

void CQanda_Shoot_Sniping::Enable_TargetUI()
{
	if (m_SnipingTarget.empty())
		return;

	auto iter = m_SnipingTarget.begin();
	for (int i = 0; i < m_SnipingTarget.size(); ++i)
	{
		CUnit* m_pTargetUnit = static_cast<CUnit*>(*iter);
		m_pTargetUnit->Get_OwnerHUD()->SetActive_TargetUI(i, true);

		iter++;
	}	
}

void CQanda_Shoot_Sniping::Disable_TargetUI()
{
	for (auto& iter : m_SnipingTarget)
	{
		CUnit* m_pTargetUnit = static_cast<CUnit*>(iter);
		m_pTargetUnit->Get_OwnerHUD()->SetActive_TargetUI(0, false);
	}
}
