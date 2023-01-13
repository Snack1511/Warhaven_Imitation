#include "stdafx.h"
#include "CState_Common_Fall_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"

CState_Common_Fall_Qanda::CState_Common_Fall_Qanda()
{
}

CState_Common_Fall_Qanda::~CState_Common_Fall_Qanda()
{
}


HRESULT CState_Common_Fall_Qanda::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_FALL_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iAINextState = AI_STATE_COMMON_LAND_QANDA;

    m_iStateChangeKeyFrame = 99;

    m_fInterPolationTime = 0.15f;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

CState_Common_Fall_Qanda* CState_Common_Fall_Qanda::Create()
{
    CState_Common_Fall_Qanda* pInstance = new CState_Common_Fall_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Fall_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CState_Common_Fall_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Fall_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Fall_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Fall_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
