#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Qanda.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Sprint_Qanda::CState_PathNavigation_Sprint_Qanda()
{
}

CState_PathNavigation_Sprint_Qanda::~CState_PathNavigation_Sprint_Qanda()
{
}

HRESULT CState_PathNavigation_Sprint_Qanda::Initialize()
{
	m_eJumpFallStateType = AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_QANDA;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    return __super::Initialize();
}

void CState_PathNavigation_Sprint_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_iStateChangeKeyFrame = 25;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

