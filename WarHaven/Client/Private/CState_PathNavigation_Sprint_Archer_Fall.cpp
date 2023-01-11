#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Archer_Fall.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Archer_Fall::CState_PathNavigation_Sprint_Archer_Fall()
{
}

CState_PathNavigation_Sprint_Archer_Fall::~CState_PathNavigation_Sprint_Archer_Fall()
{
}

CState_PathNavigation_Sprint_Archer_Fall* CState_PathNavigation_Sprint_Archer_Fall::Create()
{
    CState_PathNavigation_Sprint_Archer_Fall* pInstance = new CState_PathNavigation_Sprint_Archer_Fall();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Archer_Fall");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Archer_Fall::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 49;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Archer_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Archer_Fall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return AI_STATE_PATHNAVIGATION_SPRINTEND_ARCHER;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Archer_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Archer_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
