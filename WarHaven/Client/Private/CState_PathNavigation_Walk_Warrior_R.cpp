#include "stdafx.h"
#include "CState_PathNavigation_Walk_Warrior_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Walk_Warrior_R::CState_PathNavigation_Walk_Warrior_R()
{
}

CState_PathNavigation_Walk_Warrior_R::~CState_PathNavigation_Walk_Warrior_R()
{
}

CState_PathNavigation_Walk_Warrior_R* CState_PathNavigation_Walk_Warrior_R::Create()
{
    CState_PathNavigation_Walk_Warrior_R* pInstance = new CState_PathNavigation_Walk_Warrior_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Walk_Warrior_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Walk_Warrior_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_WALK_WARRIOR_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 45;
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 38;

    m_iAINextState = AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_WARRIOR_R;


    return S_OK;
}

void CState_PathNavigation_Walk_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Walk_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
