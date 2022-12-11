#include "stdafx.h"
#include "CState_Common_Fall_Warrior.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Fall_Warrior::CState_Common_Fall_Warrior()
{
}

CState_Common_Fall_Warrior::~CState_Common_Fall_Warrior()
{
}

HRESULT CState_Common_Fall_Warrior::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.15f;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_Common_Fall_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Fall_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Fall_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Fall_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

