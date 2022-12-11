#include "stdafx.h"
#include "CState_NoPattern.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_NoPattern::CState_NoPattern()
{
}

CState_NoPattern::~CState_NoPattern()
{
}

CState_NoPattern* CState_NoPattern::Create()
{
    CState_NoPattern* pInstance = new CState_NoPattern();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_NoPattern");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_NoPattern::Initialize()
{
    return S_OK;
}

void CState_NoPattern::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    ePrevType = m_ePreStateType;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_NoPattern::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_NoPattern::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}
