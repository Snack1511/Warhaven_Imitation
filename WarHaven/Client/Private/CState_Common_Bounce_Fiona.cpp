#include "stdafx.h"
#include "CState_Common_Bounce_Fiona.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CState_Common_Bounce_Fiona::CState_Common_Bounce_Fiona()
{
}

CState_Common_Bounce_Fiona::~CState_Common_Bounce_Fiona()
{
}

HRESULT CState_Common_Bounce_Fiona::Initialize()
{
	__super::Initialize();

	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 50;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 5.f;
	m_fMaxSpeed = 2.f;

    return S_OK;
}

void CState_Common_Bounce_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HitSmokeParticle_0"), pOwner->Get_HitMatrix());
	switch (ePrevType)
	{
	case Client::STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L:
	case Client::STATE_CHARGEATTACK_WARHAMMER_L:
	case Client::STATE_ATTACK_STING_WARHAMMER_L:

		m_iStateChangeKeyFrame = 74;
		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 0;
		m_eStateType = STATE_BOUNCE_WARHAMMER_L;

		break;

	case Client::STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R:
	case Client::STATE_CHARGEATTACK_WARHAMMER_R:
	case Client::STATE_ATTACK_STING_WARHAMMER_R:

		m_iStateChangeKeyFrame = 60;
		m_eAnimType = ANIM_BASE_R;
		m_iAnimIndex = 10;
		m_eStateType = STATE_BOUNCE_WARHAMMER_R;

		break;

		// RF
	case Client::STATE_VERTICALATTACK_WARHAMMER_R:
	case Client::STATE_CHARGEATTACK_FRONT_WARHAMMER_R:

		m_iStateChangeKeyFrame = 50;
		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 38;
		m_eStateType = STATE_BOUNCE_WARHAMMER_L;

		break;

		//LF
	case Client::STATE_SPRINTATTACK_WARHAMMER:
	case Client::STATE_VERTICALATTACK_WARHAMMER_L:
	case Client::STATE_CHARGEATTACK_FRONT_WARHAMMER_L:

		m_iStateChangeKeyFrame = 50;
		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 38;
		m_eStateType = STATE_BOUNCE_WARHAMMER_L;

		break;

	default:
		break;
	}

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
            
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Bounce_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

