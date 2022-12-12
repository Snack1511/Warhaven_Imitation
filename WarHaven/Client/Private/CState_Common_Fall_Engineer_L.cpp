#include "stdafx.h"
#include "CState_Common_Fall_Engineer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Common_Fall_Engineer_L::CState_Common_Fall_Engineer_L()
{
}

CState_Common_Fall_Engineer_L::~CState_Common_Fall_Engineer_L()
{
}

CState_Common_Fall_Engineer_L* CState_Common_Fall_Engineer_L::Create()
{
    CState_Common_Fall_Engineer_L* pInstance = new CState_Common_Fall_Engineer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Fall_Engineer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_Fall_Engineer_L::Initialize()
{
    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_FALL_ENGINEER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iAINextState = AI_STATE_COMMON_LAND_ENGINEER_L;

    return S_OK;
}

void CState_Common_Fall_Engineer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    ////if (ePrevType == STATE_JUMP_PLAYER_L)
    ////    m_fInterPolationTime = 0.05f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Fall_Engineer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Fall_Engineer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Fall_Engineer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
