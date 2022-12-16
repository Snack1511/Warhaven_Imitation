#include "stdafx.h"
#include "CState_Common_Bounce_Paladin.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CState_Common_Bounce_Paladin::CState_Common_Bounce_Paladin()
{
}

CState_Common_Bounce_Paladin::~CState_Common_Bounce_Paladin()
{
}

HRESULT CState_Common_Bounce_Paladin::Initialize()
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

void CState_Common_Bounce_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HitSmokeParticle_0"), pOwner->Get_HitMatrix());



	pOwner->Get_PhysicsCom()->Set_Speed(0.f);


	switch (ePrevType)
	{
	case Client::AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_L:


		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 0;
		m_eStateType = AI_STATE_COMMON_BOUNCE_PALADIN_L;

		break;


	case Client::AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_R:

		m_eAnimType = ANIM_BASE_R;
		m_iAnimIndex = 0;
		m_eStateType = AI_STATE_COMMON_BOUNCE_PALADIN_R;

		break;

	//case Client::AI_STATE_COMBAT_VERTICALCUT_PALADIN:

	//	m_eAnimType = ANIM_BASE_R;
	//	m_iAnimIndex = 37;
	//	m_eStateType = AI_STATE_COMMON_BOUNCE_PALADIN_R;

	//	break;

	default:
		break;
	}

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
            
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Bounce_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

