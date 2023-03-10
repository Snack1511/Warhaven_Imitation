#include "stdafx.h"
#include "CState_Common_Hit.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Hit::CState_Common_Hit()
{
}

CState_Common_Hit::~CState_Common_Hit()
{
}

HRESULT CState_Common_Hit::Initialize()
{
    __super::Initialize();

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_Common_Hit::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

    Play_Voice(pOwner, L"Voice_Hit", 1.f);
}

STATE_TYPE CState_Common_Hit::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!m_bSting)
    {
        if (pAnimator->Is_CurAnimFinished())
        {
            if(m_ePreStateType != STATE_END)
                return m_ePreStateType;

            else
            {
                STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
                return eDefaultState;
            }
            
        }
            
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Hit::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Hit::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

