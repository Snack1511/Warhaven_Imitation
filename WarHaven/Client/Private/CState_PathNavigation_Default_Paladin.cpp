#include "stdafx.h"
#include "CState_PathNavigation_Default_Paladin.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Default_Paladin::CState_PathNavigation_Default_Paladin()
{
}

CState_PathNavigation_Default_Paladin::~CState_PathNavigation_Default_Paladin()
{
}

HRESULT CState_PathNavigation_Default_Paladin::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	Add_KeyFrame(6, 0);
	Add_KeyFrame(31, 0);

    return __super::Initialize();
}

void CState_PathNavigation_Default_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

	m_iStateChangeKeyFrame = 25;

	m_eSprintBeginState = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_PALADIN;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Default_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Default_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Paladin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

