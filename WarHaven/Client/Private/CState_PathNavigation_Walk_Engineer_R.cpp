#include "stdafx.h"
#include "CState_PathNavigation_Walk_Engineer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Walk_Engineer_R::CState_PathNavigation_Walk_Engineer_R()
{
}

CState_PathNavigation_Walk_Engineer_R::~CState_PathNavigation_Walk_Engineer_R()
{
}

CState_PathNavigation_Walk_Engineer_R* CState_PathNavigation_Walk_Engineer_R::Create()
{
    CState_PathNavigation_Walk_Engineer_R* pInstance = new CState_PathNavigation_Walk_Engineer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Walk_Engineer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Walk_Engineer_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_WALK_ENGINEER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 40;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 41;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 45;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 46;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 47;

    m_iAINextState = AI_STATE_PATHNAVIGATION_DEFAULT_ENGINEER_R;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_ENGINEER_R;


    return S_OK;
}

void CState_PathNavigation_Walk_Engineer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Walk_Engineer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Walk_Engineer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Walk_Engineer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
