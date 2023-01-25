#include "stdafx.h"
#include "CState_Common.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common::CState_Common()
{
}

CState_Common::~CState_Common()
{
}

HRESULT CState_Common::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Common::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (pOwner->Get_HitType().eGroggyState != ePrevType &&
        pOwner->Get_HitType().eGuardBreakState != ePrevType &&
        pOwner->Get_HitType().eGuardState != ePrevType &&
        pOwner->Get_HitType().eFlyState != ePrevType &&
        pOwner->Get_HitType().eStingHitState != ePrevType)
    {
        m_ePreStateType = ePrevType;
    }

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    m_ePreStateType = STATE_END;

    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

