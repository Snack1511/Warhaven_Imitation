#include "stdafx.h"
#include "CRun_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Player::CRun_Player()
{
}

CRun_Player::~CRun_Player()
{
}

CRun_Player* CRun_Player::Create()
{
    CRun_Player* pInstance = new CRun_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Player::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CRun_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */

    


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CRun_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

        // Key(W)
        if (KEY(W, HOLD))
        {
            // Key(W + A)
            if (KEY(A, HOLD))
            {
               
                // AnimType->Base_R
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 29)
                    {
                        if (m_iAnimIndex != 21)
                        {
                            m_iAnimIndex = 21;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                        }

                        // RunBegin  -> Run
                        else
                        {

                            if (m_iAnimIndex == 21 && pAnimator->Is_CurAnimFinished())
                            {
                                m_iAnimIndex = 29;

                                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                            }
                        }
                    }

     
                }


                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 21)
                    {
                        if (m_iAnimIndex != 13)
                        {
                            m_iAnimIndex = 13;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                        }

                        // RunBegin  -> Run
                        else
                        {

                            if (m_iAnimIndex == 13 && pAnimator->Is_CurAnimFinished())
                            {
                                m_iAnimIndex = 21;

                                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                            }
                        }
                    }
                }
             


            }

            // Key(W + D)
            else if (KEY(D, HOLD))
            {
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 28)
                    {
                        if (m_iAnimIndex != 20)
                        {
                            m_iAnimIndex = 20;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);


                        }

                        // RunBegin -> Run
                        else
                        {
                            if (m_iAnimIndex == 20 && pAnimator->Is_CurAnimFinished())
                            {
                                m_iAnimIndex = 28;

                                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.15f);
                            }
                        }
                    }
                }

                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 20)
                    {
                        if (m_iAnimIndex != 12)
                        {
                            m_iAnimIndex = 12;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                        }

                        // RunBegin  -> Run
                        else
                        {

                            if (m_iAnimIndex == 12 && pAnimator->Is_CurAnimFinished())
                            {
                                m_iAnimIndex = 20;

                                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                            }
                        }
                    }
                }
            }

            // Key(W)
            else
            {
                if (m_eAnimType == ANIM_BASE_R)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 27)
                    {
                        if (m_iAnimIndex != 19)
                        {
                            m_iAnimIndex = 19;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);

                        }

                        // RunBegin -> Run
                        else
                        {
                            if (m_iAnimIndex == 19 && pAnimator->Is_CurAnimFinished())
                            {
                                m_iAnimIndex = 27;

                                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                            }
                        }
                    }
                }

                else if (m_eAnimType == ANIM_BASE_L)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 19)
                    {
                        if (m_iAnimIndex != 11)
                        {
                            m_iAnimIndex = 11;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);

                        }

                        // RunBegin -> Run
                        else
                        {
                            if (m_iAnimIndex == 11 && pAnimator->Is_CurAnimFinished())
                            {
                                m_iAnimIndex = 19;

                                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                            }
                        }
                    }
                }
            }


        }

        // Key(S)
        else if (KEY(S, HOLD))
        {
            // Key(S + A)
            if (KEY(A, HOLD))
            {
                if (m_eAnimType == ANIM_BASE_R)
                {
                    if (m_iAnimIndex != 44)
                    {
                        m_iAnimIndex = 44;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }

                else if (m_eAnimType == ANIM_BASE_L)
                {
                    if (m_iAnimIndex != 36)
                    {
                        m_iAnimIndex = 36;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                    }
                }

            }


            // Key(S + D)
            else if (KEY(D, HOLD))
            {
                if (m_eAnimType == ANIM_BASE_R)
                {
                    if (m_iAnimIndex != 43)
                    {
                        m_iAnimIndex = 43;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                    }
                }

                else  if (m_eAnimType == ANIM_BASE_L)
                {
                    if (m_iAnimIndex != 35)
                    {
                        m_iAnimIndex = 35;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
                    }
                }
            }



            // Key(S)
            else
            {
                if (m_eAnimType == ANIM_BASE_R)
                {
                    if (m_iAnimIndex != 42)
                    {
                        m_iAnimIndex = 42;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                    }
                }

                else if (m_eAnimType == ANIM_BASE_L)
                {

                    if (m_iAnimIndex != 34)
                    {
                        m_iAnimIndex = 34;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
                    }
                }
            }

            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
        }

        // Key(A)
        else if (KEY(A, HOLD))
        {
           
            if (m_eAnimType == ANIM_BASE_R)
            {
                if (m_iAnimIndex != 30)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 25)
                    {
                        m_iAnimIndex = 25;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                    }

                    // RunBegin -> Run
                    else
                    {
                        if (m_iAnimIndex == 25 && pAnimator->Is_CurAnimFinished())
                        {
                            m_iAnimIndex = 30;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                        }
                    }

                }
            }

          
            if (m_eAnimType == ANIM_BASE_L)
            {
                if (m_iAnimIndex != 22)
                {
                    // RunBegin 
                    if (m_iAnimIndex != 17)
                    {
                        m_iAnimIndex = 17;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                    }

                    // RunBegin -> Run
                    else
                    {
                        if (m_iAnimIndex == 17 && pAnimator->Is_CurAnimFinished())
                        {
                            m_iAnimIndex = 22;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                        }
                    }

                }
            }
          
           
           
        }

        // Key(D)
        else if (KEY(D, HOLD))
        {
            if (m_eAnimType == ANIM_BASE_R)
            {
                // RunBegin 
                if (m_iAnimIndex != 26)
                {
                    if (m_iAnimIndex != 18)
                    {
                        m_iAnimIndex = 18;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                    }

                    // RunBegin -> Run
                    else
                    {
                        if (m_iAnimIndex == 18 && pAnimator->Is_CurAnimFinished())
                        {

                            m_iAnimIndex = 26;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);

                        }
                    }

                }
            }

            else if (m_eAnimType == ANIM_BASE_L)
            {
                // RunBegin 
                if (m_iAnimIndex != 18)
                {
                    if (m_iAnimIndex != 10)
                    {
                        m_iAnimIndex = 10;

                        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
                        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                    }

                    // RunBegin -> Run
                    else
                    {
                        if (m_iAnimIndex == 10 && pAnimator->Is_CurAnimFinished())
                        {
                            m_iAnimIndex = 18;

                            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
                            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
                        }
                    }

                }
            }
           
        }
    }



    return __super::Tick(pOwner, pAnimator);
}

void CRun_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. CTRL 을 누르지 않은 상태
    2. WASD 를 누른 상태
    */

    // m_eStateType 이 End 에 가지 않으면 Enter 를 호출한다.

    // W 랑 A 를 누르면 왼쪽 앞으로 이동한다.
    // W 랑 D 를 누르면 왼쪽 옆으로 이동한다.

    // 만약 WASD 를 눌렀다면

    if (KEY(CTRL, NONE))
    {
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

//_bool CRun_Player::Change_Animation_Run(_uint iBeginAttackAnim, _uint iAttackAnim, CAnimator* pAnimator)
//{
//    // RunBegin 
//    if (m_iAnimIndex != iBeginAttackAnim)
//    {
//        if (m_iAnimIndex == 21)
//        {
//            if (m_iAnimIndex == iAttackAnim && pAnimator->Is_CurAnimFinished())
//            {
//                m_eAnimType = ANIM_BASE_R;
//                m_iAnimIndex = iBeginAttackAnim;
//
//                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
//                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
//            }
//        }
//        else
//        {
//            m_eAnimType = ANIM_BASE_R;
//            m_iAnimIndex = iAttackAnim;
//
//            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
//            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.7f);
//
//        }
//
//    return false;
//}
