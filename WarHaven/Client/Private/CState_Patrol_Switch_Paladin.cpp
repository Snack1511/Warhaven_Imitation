#include "stdafx.h"
#include "CState_Patrol_Switch_Paladin.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Switch_Paladin::CState_Patrol_Switch_Paladin()
{
}

CState_Patrol_Switch_Paladin::~CState_Patrol_Switch_Paladin()
{
}

HRESULT CState_Patrol_Switch_Paladin::Initialize()
{
	__super::Initialize();


    return S_OK;
}

void CState_Patrol_Switch_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Switch_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Switch_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Patrol_Switch_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

