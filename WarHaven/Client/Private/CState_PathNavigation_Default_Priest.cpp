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

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

	// 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 24;

    m_iAINextState = AI_STATE_PATHNAVIGATION_JUMP_PRIEST;
    m_eJumpFallStateType = AI_STATE_COMMON_FALL_PRIEST;
    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_PRIEST;

    return S_OK;
}

void CState_PathNavigation_Default_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

    m_iStateChangeKeyFrame = 25;

    m_eSprintBeginState = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_PALADIN;

    m_fRand = frandom(0.2f, 0.5f);
    m_iRand = random(0, 5);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Default_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_fAIDelayTime > m_fRand)
    {
        switch (m_iRand)
        {
        case 0:
        case 1:
        case 2:

            return m_eSprintBeginState;


        case 3:

            return m_iAINextState;

        case 4:

            if (pAnimator->Is_CurAnimFinished())
                return m_eStateType;

            break;

        case 5:

            return m_eWalkState;

            break;

        default:
            return m_iAINextState;

        }


    }

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
