#include "stdafx.h"
#include "CWalk_Player_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Player_R::CWalk_Player_R()
{
}

CWalk_Player_R::~CWalk_Player_R()
{
}

CWalk_Player_R* CWalk_Player_R::Create()
{
    CWalk_Player_R* pInstance = new CWalk_Player_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Player_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Player_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.

    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);
//    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    m_VecDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_VecDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_VecDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_VecDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
                            
    m_VecDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_VecDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_VecDirectionAnimIndex[STATE_DIRECTION_W] = 45;
    m_VecDirectionAnimIndex[STATE_DIRECTION_E] = 38;



    return S_OK;
}

void CWalk_Player_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner의 Animator Set Idle로 */

    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWalk_Player_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_PLAYER_R;
    }

    //if (
    //    KEY(W, NONE) &&
    //    KEY(A, NONE) &&
    //    KEY(S, NONE) &&
    //    KEY(D, NONE)
    //    )
    //{
    //    return STATE_IDLE_PLAYER_R;
    //}

    //else
    //{

    //    if (KEY(W, HOLD))
    //    {
    //        // Key(CTRL + W + A)
    //        if (KEY(A, HOLD))
    //        {
    //            // 예외처리r
    //            if (m_iAnimIndex != 41)
    //            {
    //                m_iAnimIndex = 41;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + W + D)
    //        else if (KEY(D, HOLD))
    //        {
    //            // 예외처리
    //            if (m_iAnimIndex != 40)
    //            {
    //                    
    //                m_iAnimIndex = 40;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + W)
    //        else
    //        {
    //            // 예외처리
    //            if (m_iAnimIndex != 39)
    //            {
    //                m_iAnimIndex = 39;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //            }
    //        }


    //    }

    //    // Key(CTRL + S)
    //    else if (KEY(S, HOLD))
    //    {
    //        // Key(CTRL + S + A)
    //        if (KEY(A, HOLD))
    //        {
    //            // 예외처리
    //            if (m_iAnimIndex != 44)
    //            {
    //                m_iAnimIndex = 44;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + S + D)
    //        else if (KEY(D, HOLD))
    //        {
    //            // 예외처리
    //            if (m_iAnimIndex != 43)
    //            {
    //                m_iAnimIndex = 43;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    //            }
    //        }

    //        // Key(CTRL + S)
    //        else
    //        {
    //            // 예외처리
    //            if (m_iAnimIndex != 42)
    //            {
    //                m_iAnimIndex = 42;

    //                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //            }
    //        }
    //    }

    //    // Key(CTRL + A)
    //    else if (KEY(A, HOLD))
    //    {
    //        // 예외처리
    //        if (m_iAnimIndex != 45)
    //        {
    //            m_iAnimIndex = 45;

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //        }
    //    }

    //    // Key(CTRL + D)
    //    else if (KEY(D, HOLD))
    //    {
    //        // 예외처리
    //        if (m_iAnimIndex != 38)
    //        {
    //            m_iAnimIndex = 38;

    //            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    //            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    //        }
    //    }
    //}

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Player_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Player_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 
    */

    // m_eStateType 이 End 에 가지 않으면 Enter 를 호출한다.

    // W 랑 A 를 누르면 왼쪽 앞으로 이동한다.
    // W 랑 D 를 누르면 왼쪽 옆으로 이동한다.

    // 만약 WASD 를 눌렀다면

    // 점프를 하지 않고
    if (KEY(SPACE, NONE))
    {
        // 천천히 
        if (KEY(CTRL, HOLD))
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
    }

   


    return STATE_END;
}
