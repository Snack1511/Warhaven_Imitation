#include "stdafx.h"
#include "CState_PathNavigation_Jump_Archer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Jump_Archer_L::CState_PathNavigation_Jump_Archer_L()
{
}

CState_PathNavigation_Jump_Archer_L::~CState_PathNavigation_Jump_Archer_L()
{
}

CState_PathNavigation_Jump_Archer_L* CState_PathNavigation_Jump_Archer_L::Create()
{
    CState_PathNavigation_Jump_Archer_L* pInstance = new CState_PathNavigation_Jump_Archer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Jump_Archer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Jump_Archer_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_JUMP_ARCHER_L; // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 99;
 
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 3;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 6;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 5;

    iPlaceJumpAnimIndex = 2;

    m_iAINextState = AI_STATE_PATHNAVIGATION_FALL_ARCHER_L;


    return S_OK;
}

void CState_PathNavigation_Jump_Archer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Archer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CState_PathNavigation_Jump_Archer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Jump_Archer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
