#include "stdafx.h"
#include "CState_PathNavigation_Default_Priest.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Default_Priest::CState_PathNavigation_Default_Priest()
{
}

CState_PathNavigation_Default_Priest::~CState_PathNavigation_Default_Priest()
{
}

CState_PathNavigation_Default_Priest* CState_PathNavigation_Default_Priest::Create()
{
    CState_PathNavigation_Default_Priest* pInstance = new CState_PathNavigation_Default_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Default_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Default_Priest::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 19;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 24;

    m_eJumpState = AI_STATE_PATHNAVIGATION_JUMP_PRIEST;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_PRIEST;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_PRIEST;
    m_eSprintBeginState = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_PRIEST;

    return S_OK;
}

void CState_PathNavigation_Default_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

    m_iStateChangeKeyFrame = 25;

    

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Default_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Default_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_PathNavigation_Default_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
