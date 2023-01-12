#include "stdafx.h"
#include "CState_Common_Fall_Fiona.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Fall_Fiona::CState_Common_Fall_Fiona()
{
}

CState_Common_Fall_Fiona::~CState_Common_Fall_Fiona()
{
}

HRESULT CState_Common_Fall_Fiona::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_Common_Fall_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Fall_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Fall_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Fall_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

