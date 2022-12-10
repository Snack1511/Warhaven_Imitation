#include "stdafx.h"
#include "CState_Combat.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat::CState_Combat()
{
}

CState_Combat::~CState_Combat()
{
}

HRESULT CState_Combat::Initialize()
{

    return S_OK;
}

void CState_Combat::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Combat::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

