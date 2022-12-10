#include "stdafx.h"
#include "CState_Patrol_Default.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Default::CState_Patrol_Default()
{
}

CState_Patrol_Default::~CState_Patrol_Default()
{
}

HRESULT CState_Patrol_Default::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Patrol_Default::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Move_Direction_Loop(pOwner, pAnimator, 0.05f);



    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CState_Patrol_Default::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

