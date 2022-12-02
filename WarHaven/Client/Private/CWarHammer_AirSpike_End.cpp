#include "stdafx.h"
#include "CWarHammer_AirSpike_End.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_AirSpike_End::CWarHammer_AirSpike_End()
{
}

CWarHammer_AirSpike_End::~CWarHammer_AirSpike_End()
{
}

CWarHammer_AirSpike_End* CWarHammer_AirSpike_End::Create()
{
	CWarHammer_AirSpike_End* pInstance = new CWarHammer_AirSpike_End();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_AirSpike_End");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_AirSpike_End::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 4.2f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 35;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_AIRSPIKE_END_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 70;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);



	m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

	Add_KeyFrame(12, 0);
	Add_KeyFrame(30, 1);
	Add_KeyFrame(35, 2);

	// return __super::Initialize();
	return S_OK;
}

void CWarHammer_AirSpike_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->CallBack_CollisionEnter += bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	Physics_Setting(0.1, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_AirSpike_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	/*if (pOwner->Is_Weapon_R_Collision() && !m_bETC_Trigger)
	{
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SmashSoilParticle", pOwner->Get_HitPos());
		m_bETC_Trigger = true;
	}*/ 


	if (m_bKeyInputable)
	{
		if (KEY(LBUTTON, TAP))
			m_bKeyInput = true;
		
		else if (MOUSE_MOVE(MMS_WHEEL) < 0)
			m_bKeyInputWheel = true;
	}

	if (m_bHit && m_bKeyInput)
	{

		if (m_bKeyInput)
			return STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L;

		else if (m_bKeyInputWheel)
			return STATE_VERTICALATTACK_WARHAMMER_L;

	}


	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_AirSpike_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->CallBack_CollisionEnter -= bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	pOwner->Enable_FlyAttackCollider(false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CWarHammer_AirSpike_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{




	/* WARHAMMER가 AirSpike_End 로 오는 조건
	1.  지면에 떨어지면
	*/
	if (!pOwner->Is_Air())
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CWarHammer_AirSpike_End::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
	if (iOtherColType == COL_REDHITBOX_BODY ||
		iOtherColType == COL_REDHITBOX_HEAD ||
		iOtherColType == COL_REDGUARD)
		m_bHit = true;
}

void CWarHammer_AirSpike_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		pOwner->Shake_Camera(pOwner->Get_Status().fCamPower , pOwner->Get_Status().fCamTime);
		m_bAttackTrigger = true;
		pOwner->Enable_FlyAttackCollider(true);
		break;

	case 1:
		m_bAttackTrigger = false;
		m_bKeyInputable = true;
		pOwner->Enable_FlyAttackCollider(false);
		break;



	case 2:
		
		break;

	default:
		break;
	}

}
