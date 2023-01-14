#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Warrior_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Warrior_Begin::CState_PathNavigation_Sprint_Warrior_Begin()
{
}

CState_PathNavigation_Sprint_Warrior_Begin::~CState_PathNavigation_Sprint_Warrior_Begin()
{
}

CState_PathNavigation_Sprint_Warrior_Begin* CState_PathNavigation_Sprint_Warrior_Begin::Create()
{
    CState_PathNavigation_Sprint_Warrior_Begin* pInstance = new CState_PathNavigation_Sprint_Warrior_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Warrior_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Warrior_Begin::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 55;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    m_fInterPolationTime = 0.1f;

    Add_KeyFrame(23, 0, true);
    Add_KeyFrame(42, 0, true);
    Add_KeyFrame(58, 0, true);
    Add_KeyFrame(75, 0, true);
    Add_KeyFrame(93, 0, true);
    Add_KeyFrame(108, 0, true);

    return S_OK;
}

void CState_PathNavigation_Sprint_Warrior_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_SPRINTLOOP_WARRIOR;

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Warrior_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
