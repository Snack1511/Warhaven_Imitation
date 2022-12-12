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
	__super::Initialize();

	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_Combat_Default_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == m_eStateType)
		m_fInterPolationTime = 0.f;

	m_iRand = random(0, 7);
	m_iDirectionRand = random(0, 7);

	Set_Direction_Front_AI(m_iDirectionRand);
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iStateChangeKeyFrame = 15;

	m_fAIMyLength = 2.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iFrame = pAnimator->Get_CurAnimFrame() + 1;

	switch (m_iDirectionRand)
	{
	case 0:
	case 1:
	case 2:

		if (iFrame == m_iStateChangeKeyFrame)
			return m_eStateType;

		break;

	case 3:
	case 4:
	case 5:
	case 6:
	case 7:

		if (iFrame == _uint(_float(m_iStateChangeKeyFrame) * 1.5f))
			return m_eStateType;

		break;

	default:
		break;
	}


	DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Combat_Default_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

