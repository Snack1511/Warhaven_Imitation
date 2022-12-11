#include "stdafx.h"
#include "CState_PathNavigation_Run.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Run::CState_PathNavigation_Run()
{
}

CState_PathNavigation_Run::~CState_PathNavigation_Run()
{
}

HRESULT CState_PathNavigation_Run::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_PathNavigation_Run::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Run::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Run::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Run::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

