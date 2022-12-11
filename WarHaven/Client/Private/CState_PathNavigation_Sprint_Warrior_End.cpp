#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Warrior_End.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Warrior_End::CState_PathNavigation_Sprint_Warrior_End()
{
}

CState_PathNavigation_Sprint_Warrior_End::~CState_PathNavigation_Sprint_Warrior_End()
{
}

CState_PathNavigation_Sprint_Warrior_End* CState_PathNavigation_Sprint_Warrior_End::Create()
{
    CState_PathNavigation_Sprint_Warrior_End* pInstance = new CState_PathNavigation_Sprint_Warrior_End();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Warrior_End");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Warrior_End::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 56;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Warrior_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fRand = frandom(0.3f, 0.8f);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_fAIDelayTime > m_fRand)
        return AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R;


    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Warrior_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
