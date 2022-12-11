#include "stdafx.h"
#include "CState_Patrol_Switch.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Switch::CState_Patrol_Switch()
{
}

CState_Patrol_Switch::~CState_Patrol_Switch()
{
}

HRESULT CState_Patrol_Switch::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Patrol_Switch::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Switch::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Switch::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Switch::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

