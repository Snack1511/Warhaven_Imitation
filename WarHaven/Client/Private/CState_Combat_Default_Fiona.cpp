#include "stdafx.h"
#include "CState_Combat_Default_Fiona.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Default_Fiona::CState_Combat_Default_Fiona()
{
}

CState_Combat_Default_Fiona::~CState_Combat_Default_Fiona()
{
}

HRESULT CState_Combat_Default_Fiona::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;


    return __super::Initialize();
}

void CState_Combat_Default_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_Status().fDamageMultiplier = 2.f;

	if (ePrevType == m_eStateType)
		m_fInterPolationTime = 0.f;

	m_iRand = random(0, 7);
	m_iDirectionRand = STATE_DIRECTION_N;//random(0, 7);

	
	//Set_Direction_Front_AI(m_iDirectionRand);
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iStateChangeKeyFrame = 99;

	m_fAIMyLength = 2.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* ?Ұž??? */
}

STATE_TYPE CState_Combat_Default_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Default_Fiona::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

