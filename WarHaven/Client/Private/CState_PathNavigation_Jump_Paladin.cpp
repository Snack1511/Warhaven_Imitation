#include "stdafx.h"
#include "CState_PathNavigation_Jump_Paladin.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Jump_Paladin::CState_PathNavigation_Jump_Paladin()
{
}

CState_PathNavigation_Jump_Paladin::~CState_PathNavigation_Jump_Paladin()
{
}

HRESULT CState_PathNavigation_Jump_Paladin::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.f;
	

    return S_OK;
}

void CState_PathNavigation_Jump_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Jump_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Jump_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

