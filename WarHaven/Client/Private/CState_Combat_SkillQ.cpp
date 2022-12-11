#include "stdafx.h"
#include "CState_Combat_SkillQ.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_SkillQ::CState_Combat_SkillQ()
{
}

CState_Combat_SkillQ::~CState_Combat_SkillQ()
{
}

HRESULT CState_Combat_SkillQ::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Combat_SkillQ::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillQ::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

