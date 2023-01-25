#include "stdafx.h"
#include "CState_Combat_Shoot_Qanda.h"

#include "UsefulHeaders.h"
#include "CPhysXCharacter.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_Qanda.h"
#include "CAnimWeapon.h"
#include "CAnimWeapon_Crow.h"


CState_Combat_Shoot_Qanda::CState_Combat_Shoot_Qanda()
{
}

CState_Combat_Shoot_Qanda::~CState_Combat_Shoot_Qanda()
{
}

CState_Combat_Shoot_Qanda* CState_Combat_Shoot_Qanda::Create()
{
    CState_Combat_Shoot_Qanda* pInstance = new CState_Combat_Shoot_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Shoot_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Shoot_Qanda::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 3.f;
	m_tHitInfo.iLandKeyFrame = 60;
	m_tHitInfo.bFly = true;
	m_tHitInfo.bNoneHeadAttack = true;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 40;


    return __super::Initialize();
}

void CState_Combat_Shoot_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP1)
		m_fDamagePumping = 2.f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP2)
		m_fDamagePumping = 2.5f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP3)
	{
		m_fDamagePumping =	4.f;
		m_iAnimIndex = 8;
	} 

	if (ePrevType == AI_STATE_COMBAT_BEGIN_SNIPING_QANDA ||
		ePrevType == AI_STATE_COMBAT_BEGIN_QANDA)
		m_fDamagePumping = 1.5f;

	pOwner->Get_Status().fDamageMultiplier = m_fDamagePumping;

	static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(16, m_fInterPolationTime, m_fAnimSpeed);
	static_cast<CUnit_Qanda*>(pOwner)->Shoot_AnimCrow();

	_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);


	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);
    
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Shoot_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEAFULT_QANDA;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Shoot_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP_END;
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Shoot_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Shoot_Qanda::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{

	case 1:
		m_bAttackTrigger = true;
		break;

	default:
		break;
	}

}