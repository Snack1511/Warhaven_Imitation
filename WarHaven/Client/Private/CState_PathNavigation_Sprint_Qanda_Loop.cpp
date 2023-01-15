#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Qanda_Loop.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Qanda_Loop::CState_PathNavigation_Sprint_Qanda_Loop()
{
}

CState_PathNavigation_Sprint_Qanda_Loop::~CState_PathNavigation_Sprint_Qanda_Loop()
{
}

CState_PathNavigation_Sprint_Qanda_Loop* CState_PathNavigation_Sprint_Qanda_Loop::Create()
{
    CState_PathNavigation_Sprint_Qanda_Loop* pInstance = new CState_PathNavigation_Sprint_Qanda_Loop();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Qanda_Loop");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Qanda_Loop::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 36;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTLOOP_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_QANDA;

    Add_KeyFrame(10, 0, true);
    Add_KeyFrame(27, 0, true);
    Add_KeyFrame(43, 0, true);
    Add_KeyFrame(60, 0, true);
    return __super::Initialize();
}

void CState_PathNavigation_Sprint_Qanda_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fRand = frandom(2.1f, 8.f);
    m_iRand = random(SPRINT_STATE_LOOP1, SPRINT_STATE_END);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_fAIDelayTime > m_fRand)
    {
        switch (m_iRand)
        {
        case SPRINT_STATE_LOOP1:
        case SPRINT_STATE_LOOP2:
        case SPRINT_STATE_LOOP3:
           
            m_iRand = random(SPRINT_STATE_LOOP1, SPRINT_STATE_END);

            break;

        case SPRINT_STATE_JUMP:


            return AI_STATE_PATHNAVIGATION_SPRINTJUMP_QANDA;

        case SPRINT_STATE_STOP:

            return AI_STATE_PATHNAVIGATION_SPRINTEND_QANDA;

        default:

            m_iRand = random(SPRINT_STATE_LOOP1, SPRINT_STATE_END);

            break;
        }

            
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Qanda_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
