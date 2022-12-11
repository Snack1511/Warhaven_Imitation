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

void CState_Common::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_ePreStateType = ePrevType;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Common::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

