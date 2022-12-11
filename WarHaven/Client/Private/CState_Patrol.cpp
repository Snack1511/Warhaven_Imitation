#include "stdafx.h"
#include "CState_Patrol.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol::CState_Patrol()
{
}

CState_Patrol::~CState_Patrol()
{
}

HRESULT CState_Patrol::Initialize()
{

    return S_OK;
}

void CState_Patrol::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Patrol::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

