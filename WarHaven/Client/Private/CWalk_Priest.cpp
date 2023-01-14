#include "stdafx.h"
#include "CWalk_Priest.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Priest::CWalk_Priest()
{
}

CWalk_Priest::~CWalk_Priest()
{
}

CWalk_Priest* CWalk_Priest::Create()
{
    CWalk_Priest* pInstance = new CWalk_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Priest::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 100.f;


    m_vecAdjState.push_back(STATE_IDLE_PRIEST);
    m_vecAdjState.push_back(STATE_RUN_PRIEST);
    m_vecAdjState.push_back(STATE_JUMP_PRIEST);

    Init_AttackState_Priest();
    Init_CommonState_Player();


	// 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;

    m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    Add_KeyFrame(15, 0);
    Add_KeyFrame(50, 0);

    return S_OK;
}

void CWalk_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */
    m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Move_Direction_Loop(pOwner, pAnimator, 0.05f);

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PRIEST;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_PRIEST;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(C, HOLD))
    {
        // 걸어간다.
        if (KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD))
        {

            return m_eStateType;
        }

    }

    return STATE_END;
}

void CWalk_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
        break;
    }
}