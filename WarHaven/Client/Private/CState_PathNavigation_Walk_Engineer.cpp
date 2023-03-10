#include "stdafx.h"
#include "CState_PathNavigation_Walk_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Walk_Engineer::CState_PathNavigation_Walk_Engineer()
{
}

CState_PathNavigation_Walk_Engineer::~CState_PathNavigation_Walk_Engineer()
{
}

HRESULT CState_PathNavigation_Walk_Engineer::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 0;

	m_fAnimSpeed = 2.5f;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	Add_KeyFrame(14, 0, true);
	Add_KeyFrame(59, 0, true);
    return S_OK;
}

void CState_PathNavigation_Walk_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));
	m_iRand = random(0, 2);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_iRand != 0)
		return m_eStateType;
	else
		return m_iAINextState;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* ?Ұž??? */
}

STATE_TYPE CState_PathNavigation_Walk_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

