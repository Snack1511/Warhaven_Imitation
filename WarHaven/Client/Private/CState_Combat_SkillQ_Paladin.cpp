#include "stdafx.h"
#include "CState_Combat_SkillQ_Paladin.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_SkillQ_Paladin::CState_Combat_SkillQ_Paladin()
{
}

CState_Combat_SkillQ_Paladin::~CState_Combat_SkillQ_Paladin()
{
}

HRESULT CState_Combat_SkillQ_Paladin::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_Combat_SkillQ_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_iDirectionRand = STATE_DIRECTION_N;

    
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillQ_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

