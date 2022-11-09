#include "stdafx.h"
#include "CWalk_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Player::CWalk_Player()
{
}

CWalk_Player::~CWalk_Player()
{
}

CWalk_Player* CWalk_Player::Create()
{
    CWalk_Player* pInstance = new CWalk_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Player::Initialize()
{



    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CWalk_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */

    


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CWalk_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
        return STATE_IDLE_PLAYER;
    else
    {

        if (KEY(W, HOLD))
        {
            // Key(CTRL + W + A)
            if (KEY(A, HOLD))
            {
                // Base_R
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // 예외처리r
                    if (m_iAnimIndex != 41)
                    {
                        m_iAnimIndex = 41;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }

                //Base_L
                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // 예외처리
                    if (m_iAnimIndex != 33)
                    {
                        m_iAnimIndex = 33;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }

            }

            // Key(CTRL + W + D)
            else if (KEY(D, HOLD))
            {
                // Base_R
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // 예외처리
                    if (m_iAnimIndex != 40)
                    {
                        
                        m_iAnimIndex = 40;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }

                // Base_L
                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // 예외처리
                    if (m_iAnimIndex != 32)
                    {
                        m_iAnimIndex = 32;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }
            }

            // Key(CTRL + W)
            else
            {
                // Base_R
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // 예외처리
                    if (m_iAnimIndex != 39)
                    {
                        m_iAnimIndex = 39;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                    }
                }

                //Base_L
                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // 예외처리
                    if (m_iAnimIndex != 31)
                    {
                        m_iAnimIndex = 31;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                    }
                }
            }


        }

        // Key(CTRL + S)
        else if (KEY(S, HOLD))
        {
            // Key(CTRL + S + A)
            if (KEY(A, HOLD))
            {
                // Base_R
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // 예외처리
                    if (m_iAnimIndex != 44)
                    {
                        m_iAnimIndex = 44;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }

                //Base_L
                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // 예외처리
                    if (m_iAnimIndex != 36)
                    {
                        m_iAnimIndex = 36;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }
            }

            // Key(CTRL + S + D)
            else if (KEY(D, HOLD))
            {
                // Base_R
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // 예외처리
                    if (m_iAnimIndex != 43)
                    {
                        m_iAnimIndex = 43;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }

                //Base_L
                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // 예외처리
                    if (m_iAnimIndex != 35)
                    {
                        m_iAnimIndex = 35;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }
            }

            // Key(CTRL + S)
            else
            {
                // Base_R
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // 예외처리
                    if (m_iAnimIndex != 42)
                    {
                        m_iAnimIndex = 42;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                    }

                }

                //Base_L
                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // 예외처리
                    if (m_iAnimIndex != 34)
                    {
                        m_iAnimIndex = 34;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }
            }
        }

        // Key(CTRL + A)
        else if (KEY(A, HOLD))
        {
            // Base_R
            if (m_eAnimType == ANIM_BASE_R) 
            {
                // 예외처리
                if (m_iAnimIndex != 45)
                {
                    m_iAnimIndex = 45;

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                }

             
               
            }

            //Base_L
            else if (m_eAnimType == ANIM_BASE_L)
            {
                // 예외처리
                if (m_iAnimIndex != 37)
                {
                    m_iAnimIndex = 37;

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                }
            }
        }

        // Key(CTRL + D)
        else if (KEY(D, HOLD))
        {
        // Base_R
            if (m_eAnimType == ANIM_BASE_R)
            {
                // 예외처리
                if (m_iAnimIndex != 38)
                {
                    m_iAnimIndex = 38;

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                }
            }

            //Base_L
            else if (m_eAnimType == ANIM_BASE_L)
            {
                // 예외처리
                if (m_iAnimIndex != 30)
                {
                    m_iAnimIndex = 30;

                    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                }
            }
        }
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
   
        //CTRL 로 바꾸셈.
      
      

   


    return STATE_END;
}
