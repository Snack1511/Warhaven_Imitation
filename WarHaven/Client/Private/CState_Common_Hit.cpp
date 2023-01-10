#include "stdafx.h"
#include "CState_Common_Hit.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Hit::CState_Common_Hit()
{
}

CState_Common_Hit::~CState_Common_Hit()
{
}

HRESULT CState_Common_Hit::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Common_Hit::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Hit::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!m_bSting)
    {
        if (pAnimator->Is_CurAnimFinished())
            return m_ePreStateType;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Hit::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Hit::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

