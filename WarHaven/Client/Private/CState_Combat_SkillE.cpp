#include "stdafx.h"
#include "CState_Combat_SkillE.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_SkillE::CState_Combat_SkillE()
{
}

CState_Combat_SkillE::~CState_Combat_SkillE()
{
}

HRESULT CState_Combat_SkillE::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Combat_SkillE::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillE::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

