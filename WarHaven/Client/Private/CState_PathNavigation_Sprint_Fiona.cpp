#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Fiona.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Sprint_Fiona::CState_PathNavigation_Sprint_Fiona()
{
}

CState_PathNavigation_Sprint_Fiona::~CState_PathNavigation_Sprint_Fiona()
{
}

HRESULT CState_PathNavigation_Sprint_Fiona::Initialize()
{
	__super::Initialize();

	m_eJumpFallStateType = AI_STATE_COMMON_FALL_FIONA_R;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	
	m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_FIONA_R;

    return S_OK;
}

void CState_PathNavigation_Sprint_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_iStateChangeKeyFrame = 25;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Sprint_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

