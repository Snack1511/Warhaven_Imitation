#include "stdafx.h"
#include "CState_Combat_SkillR.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_SkillR::CState_Combat_SkillR()
{
}

CState_Combat_SkillR::~CState_Combat_SkillR()
{
}

HRESULT CState_Combat_SkillR::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Combat_SkillR::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillR::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillR::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillR::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

