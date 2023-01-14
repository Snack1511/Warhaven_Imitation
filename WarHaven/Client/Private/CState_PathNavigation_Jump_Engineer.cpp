#include "stdafx.h"
#include "CState_PathNavigation_Jump_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Jump_Engineer::CState_PathNavigation_Jump_Engineer()
{
}

CState_PathNavigation_Jump_Engineer::~CState_PathNavigation_Jump_Engineer()
{
}

HRESULT CState_PathNavigation_Jump_Engineer::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_PathNavigation_Jump_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Jump_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Jump_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_PathNavigation_Jump_Engineer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

