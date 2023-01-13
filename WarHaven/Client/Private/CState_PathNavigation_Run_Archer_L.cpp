#include "stdafx.h"
#include "CState_PathNavigation_Run_Archer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Run_Archer_L::CState_PathNavigation_Run_Archer_L()
{
}

CState_PathNavigation_Run_Archer_L::~CState_PathNavigation_Run_Archer_L()
{
}

CState_PathNavigation_Run_Archer_L* CState_PathNavigation_Run_Archer_L::Create()
{
    CState_PathNavigation_Run_Archer_L* pInstance = new CState_PathNavigation_Run_Archer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Run_Archer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Run_Archer_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 18;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_ARCHER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 18;

    m_eJumpState = AI_STATE_PATHNAVIGATION_JUMP_ARCHER_L;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_ARCHER_L;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_ARCHER_L;

    return S_OK;
}

void CState_PathNavigation_Run_Archer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Run_Archer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Run_Archer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Run_Archer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
