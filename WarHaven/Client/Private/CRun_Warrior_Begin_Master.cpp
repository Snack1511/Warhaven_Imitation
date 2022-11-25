#include "stdafx.h"
#include "CRun_Warrior_Begin_Master.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Warrior_Begin_Master::CRun_Warrior_Begin_Master()
{
}

CRun_Warrior_Begin_Master::~CRun_Warrior_Begin_Master()
{
}

HRESULT CRun_Warrior_Begin_Master::Initialize()
{
    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.f;


	m_fMyAccel = 10.f;
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 21;

	m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CRun_Warrior_Begin_Master::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Warrior_Begin_Master::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CRun_Warrior_Begin_Master::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_Warrior_Begin_Master::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
