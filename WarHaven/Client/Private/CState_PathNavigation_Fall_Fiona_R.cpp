#include "stdafx.h"
#include "CState_PathNavigation_Fall_Fiona_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"

CState_PathNavigation_Fall_Fiona_R::CState_PathNavigation_Fall_Fiona_R()
{
}

CState_PathNavigation_Fall_Fiona_R::~CState_PathNavigation_Fall_Fiona_R()
{
}


HRESULT CState_PathNavigation_Fall_Fiona_R::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_FALL_FIONA_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_eAILandStateType = AI_STATE_COMMON_LAND_FIONA_R;

    return __super::Initialize();
}

CState_PathNavigation_Fall_Fiona_R* CState_PathNavigation_Fall_Fiona_R::Create()
{
    CState_PathNavigation_Fall_Fiona_R* pInstance = new CState_PathNavigation_Fall_Fiona_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Fall_Fiona_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CState_PathNavigation_Fall_Fiona_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Fall_Fiona_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Fall_Fiona_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Fall_Fiona_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
