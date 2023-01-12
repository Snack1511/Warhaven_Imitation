#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Archer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Sprint_Archer::CState_PathNavigation_Sprint_Archer()
{
}

CState_PathNavigation_Sprint_Archer::~CState_PathNavigation_Sprint_Archer()
{
}

HRESULT CState_PathNavigation_Sprint_Archer::Initialize()
{
	__super::Initialize();

	m_eJumpFallStateType = AI_STATE_COMMON_FALL_WARRIOR_R;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;	

    return S_OK;
}

void CState_PathNavigation_Sprint_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_iStateChangeKeyFrame = 25;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Sprint_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

