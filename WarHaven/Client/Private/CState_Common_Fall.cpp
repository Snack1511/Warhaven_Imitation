#include "stdafx.h"
#include "CState_Common_Fall.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Fall::CState_Common_Fall()
{
}

CState_Common_Fall::~CState_Common_Fall()
{
}

HRESULT CState_Common_Fall::Initialize()
{
    return S_OK;
}

void CState_Common_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Fall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return m_iAINextState;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

