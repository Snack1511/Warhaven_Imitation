#include "stdafx.h"
#include "CState_PathNavigation_Fall_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Fall_Engineer::CState_PathNavigation_Fall_Engineer()
{
}

CState_PathNavigation_Fall_Engineer::~CState_PathNavigation_Fall_Engineer()
{
}

HRESULT CState_PathNavigation_Fall_Engineer::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_PathNavigation_Fall_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Fall_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Fall_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Fall_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

