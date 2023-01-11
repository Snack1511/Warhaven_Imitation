#include "stdafx.h"
#include "CState_PathNavigation_Run_Archer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Run_Archer_R::CState_PathNavigation_Run_Archer_R()
{
}

CState_PathNavigation_Run_Archer_R::~CState_PathNavigation_Run_Archer_R()
{
}

CState_PathNavigation_Run_Archer_R* CState_PathNavigation_Run_Archer_R::Create()
{
    CState_PathNavigation_Run_Archer_R* pInstance = new CState_PathNavigation_Run_Archer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Run_Archer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Run_Archer_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 27;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;


    m_fAnimSpeed = 2.5f;

	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 26;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 27;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 28;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 29;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 27;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 27;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 27;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 32;

    m_iAINextState = AI_STATE_PATHNAVIGATION_JUMP_ARCHER_R;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_ARCHER_R;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_ARCHER_R;

    return S_OK;
}

void CState_PathNavigation_Run_Archer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Run_Archer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Run_Archer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Run_Archer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
