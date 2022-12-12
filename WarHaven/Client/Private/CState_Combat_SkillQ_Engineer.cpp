#include "stdafx.h"
#include "CState_Combat_SkillQ_Engineer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_SkillQ_Engineer::CState_Combat_SkillQ_Engineer()
{
}

CState_Combat_SkillQ_Engineer::~CState_Combat_SkillQ_Engineer()
{
}

HRESULT CState_Combat_SkillQ_Engineer::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_Combat_SkillQ_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_iDirectionRand = STATE_DIRECTION_N;

    
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillQ_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

