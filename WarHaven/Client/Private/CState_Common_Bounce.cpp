#include "stdafx.h"
#include "CState_Common_Bounce.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Bounce::CState_Common_Bounce()
{
}

CState_Common_Bounce::~CState_Common_Bounce()
{
}

HRESULT CState_Common_Bounce::Initialize()
{
    // __super::Initialize();

    return S_OK;
}

void CState_Common_Bounce::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Bounce::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

