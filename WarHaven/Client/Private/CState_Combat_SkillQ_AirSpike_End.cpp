#include "stdafx.h"
#include "CState_Combat_SkillQ_AirSpike_End.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_SkillQ_AirSpike_End::CState_Combat_SkillQ_AirSpike_End()
{
}

CState_Combat_SkillQ_AirSpike_End::~CState_Combat_SkillQ_AirSpike_End()
{
}

CState_Combat_SkillQ_AirSpike_End* CState_Combat_SkillQ_AirSpike_End::Create()
{
	CState_Combat_SkillQ_AirSpike_End* pInstance = new CState_Combat_SkillQ_AirSpike_End();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_AirSpike_End");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillQ_AirSpike_End::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 4.2f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 35;


	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_AIRSPIKE_END_ENGINEER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 70;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;


	Add_KeyFrame(12, 0);
	Add_KeyFrame(30, 1);
	Add_KeyFrame(45, 2);

	return __super::Initialize();
}

void CState_Combat_SkillQ_AirSpike_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = random(0, 3);

	pOwner->CallBack_CollisionEnter += bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;
	pOwner->Get_PhysicsCom()->Set_MaxSpeed(0.1f);

	Physics_Setting_AI(0.1f, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_AirSpike_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEAFULT_ENGINEER_R;


	if (m_bHit && m_bKeyInputable)
	{
		switch (m_iRand)
		{
		case 0:

			return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_L;

		case 1:

			return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R;

		case 2:

			return AI_STATE_COMBAT_VERTICALCUT_ENGINEER_L;

		case 3:

			return AI_STATE_COMBAT_VERTICALCUT_ENGINEER_R;

		default:
			break;
		}

			
	}


	return CState_Combat_SkillQ::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_AirSpike_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->CallBack_CollisionEnter -= bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	pOwner->Enable_FlyAttackCollider(false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_SkillQ_AirSpike_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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

void CState_Combat_SkillQ_AirSpike_End::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
	if (iOtherColType == COL_REDHITBOX_BODY ||
		iOtherColType == COL_REDHITBOX_HEAD ||
		iOtherColType == COL_REDGUARD ||
		iOtherColType == COL_BLUEHITBOX_BODY ||
		iOtherColType == COL_BLUEHITBOX_HEAD ||
		iOtherColType == COL_BLUEGUARD)
		m_bHit = true;
}

void CState_Combat_SkillQ_AirSpike_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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
