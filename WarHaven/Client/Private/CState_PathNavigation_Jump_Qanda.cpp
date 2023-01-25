#include "stdafx.h"
#include "CState_PathNavigation_Jump_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Jump_Qanda::CState_PathNavigation_Jump_Qanda()
{
}

CState_PathNavigation_Jump_Qanda::~CState_PathNavigation_Jump_Qanda()
{
}

CState_PathNavigation_Jump_Qanda* CState_PathNavigation_Jump_Qanda::Create()
{
    CState_PathNavigation_Jump_Qanda* pInstance = new CState_PathNavigation_Jump_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Jump_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_PathNavigation_Jump_Qanda::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_JUMP_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 3;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 6;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 5;


	iPlaceJumpAnimIndex = 2;

    m_eFallState = AI_STATE_PATHNAVIGATION_FALL_QANDA;


    return S_OK;
}

void CState_PathNavigation_Jump_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   
    return __super::Tick(pOwner, pAnimator);
    
}

void CState_PathNavigation_Jump_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Jump_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}

void CState_PathNavigation_Jump_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
