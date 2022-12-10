#include "stdafx.h"
#include "CState_Combat_Run.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Run::CState_Combat_Run()
{
}

CState_Combat_Run::~CState_Combat_Run()
{
}

HRESULT CState_Combat_Run::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Combat_Run::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Run::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Run::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Run::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

