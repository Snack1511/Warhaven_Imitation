#include "stdafx.h"
#include "CState_PathNavigation_Default_Paladin_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Default_Paladin_R::CState_PathNavigation_Default_Paladin_R()
{
}

CState_PathNavigation_Default_Paladin_R::~CState_PathNavigation_Default_Paladin_R()
{
}

CState_PathNavigation_Default_Paladin_R* CState_PathNavigation_Default_Paladin_R::Create()
{
    CState_PathNavigation_Default_Paladin_R* pInstance = new CState_PathNavigation_Default_Paladin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Default_Paladin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Default_Paladin_R::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 19;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;

    m_eJumpState = AI_STATE_PATHNAVIGATION_JUMP_PALADIN_R;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_PALADIN_R;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_PALADIN_R;

    return __super::Initialize();
}

void CState_PathNavigation_Default_Paladin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Default_Paladin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Paladin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Default_Paladin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
