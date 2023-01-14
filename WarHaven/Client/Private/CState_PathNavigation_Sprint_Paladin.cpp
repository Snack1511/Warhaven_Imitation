#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Paladin.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Sprint_Paladin::CState_PathNavigation_Sprint_Paladin()
{
}

CState_PathNavigation_Sprint_Paladin::~CState_PathNavigation_Sprint_Paladin()
{
}

HRESULT CState_PathNavigation_Sprint_Paladin::Initialize()
{
	__super::Initialize();

	m_eJumpFallStateType = AI_STATE_PATHNAVIGATION_SPRINTEND_PALADIN;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_iStateChangeKeyFrame = 25;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Sprint_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_PathNavigation_Sprint_Paladin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

