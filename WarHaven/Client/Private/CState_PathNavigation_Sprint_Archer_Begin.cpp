#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Archer_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Archer_Begin::CState_PathNavigation_Sprint_Archer_Begin()
{
}

CState_PathNavigation_Sprint_Archer_Begin::~CState_PathNavigation_Sprint_Archer_Begin()
{
}

CState_PathNavigation_Sprint_Archer_Begin* CState_PathNavigation_Sprint_Archer_Begin::Create()
{
    CState_PathNavigation_Sprint_Archer_Begin* pInstance = new CState_PathNavigation_Sprint_Archer_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Archer_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Archer_Begin::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 47;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Archer_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Archer_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_SPRINTLOOP_ARCHER;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Archer_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Archer_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
