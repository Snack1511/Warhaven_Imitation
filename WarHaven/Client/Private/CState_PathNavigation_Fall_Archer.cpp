#include "stdafx.h"
#include "CState_PathNavigation_Fall_Archer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Fall_Archer::CState_PathNavigation_Fall_Archer()
{
}

CState_PathNavigation_Fall_Archer::~CState_PathNavigation_Fall_Archer()
{
}

HRESULT CState_PathNavigation_Fall_Archer::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 99;

    m_fInterPolationTime = 0.15f;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Fall_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Fall_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Fall_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Fall_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

