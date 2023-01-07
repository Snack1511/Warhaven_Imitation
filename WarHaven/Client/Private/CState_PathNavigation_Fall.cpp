#include "stdafx.h"
#include "CState_PathNavigation_Fall.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Fall::CState_PathNavigation_Fall()
{
}

CState_PathNavigation_Fall::~CState_PathNavigation_Fall()
{
}

HRESULT CState_PathNavigation_Fall::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_PathNavigation_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Fall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return m_eAILandStateType;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

