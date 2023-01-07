#include "stdafx.h"
#include "CState_PathNavigation_Fall_Fiona_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Fall_Fiona_L::CState_PathNavigation_Fall_Fiona_L()
{
}

CState_PathNavigation_Fall_Fiona_L::~CState_PathNavigation_Fall_Fiona_L()
{
}

CState_PathNavigation_Fall_Fiona_L* CState_PathNavigation_Fall_Fiona_L::Create()
{
    CState_PathNavigation_Fall_Fiona_L* pInstance = new CState_PathNavigation_Fall_Fiona_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Fall_Fiona_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_PathNavigation_Fall_Fiona_L::Initialize()
{
    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_FALL_FIONA_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_eAILandStateType = AI_STATE_COMMON_LAND_FIONA_L;

    return __super::Initialize();
}

void CState_PathNavigation_Fall_Fiona_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Fall_Fiona_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Fall_Fiona_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Fall_Fiona_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
