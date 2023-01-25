#include "stdafx.h"
#include "CState_PathNavigation_Walk_Paladin.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Walk_Paladin::CState_PathNavigation_Walk_Paladin()
{
}

CState_PathNavigation_Walk_Paladin::~CState_PathNavigation_Walk_Paladin()
{
}

HRESULT CState_PathNavigation_Walk_Paladin::Initialize()
{
	__super::Initialize();

	m_iAnimIndex = 30;

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

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 30;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 31;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;

	Add_KeyFrame(33, 0, true);
	Add_KeyFrame(67, 0, true);
    return S_OK;
}

void CState_PathNavigation_Walk_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));
	m_iRand = random(0, 2);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_iRand != 0)
		return m_eStateType;
	else
		return m_iAINextState;


    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Walk_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

