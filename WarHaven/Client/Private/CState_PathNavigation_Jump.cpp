#include "stdafx.h"
#include "CState_PathNavigation_Jump.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Jump::CState_PathNavigation_Jump()
{
}

CState_PathNavigation_Jump::~CState_PathNavigation_Jump()
{
}

HRESULT CState_PathNavigation_Jump::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_PathNavigation_Jump::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

    Set_Direction_Front_AI(m_iDirectionRand);

    m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
    pOwner->Get_PhysicsCom()->Set_SpeedasMax();
    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air() || pAnimator->Is_CurAnimFinished())
        return m_iAINextState;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Jump::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Jump::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

