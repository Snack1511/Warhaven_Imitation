#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Sprint_Engineer::CState_PathNavigation_Sprint_Engineer()
{
}

CState_PathNavigation_Sprint_Engineer::~CState_PathNavigation_Sprint_Engineer()
{
}

HRESULT CState_PathNavigation_Sprint_Engineer::Initialize()
{
	m_eJumpFallStateType = AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_ENGINEER;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    return __super::Initialize();
}

void CState_PathNavigation_Sprint_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_iStateChangeKeyFrame = 25;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* ?Ұž??? */
}

STATE_TYPE CState_PathNavigation_Sprint_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Engineer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

