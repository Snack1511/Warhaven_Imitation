#include "stdafx.h"
#include "CRun_Player_Begin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Player_Begin::CRun_Player_Begin()
{
}

CRun_Player_Begin::~CRun_Player_Begin()
{
}

HRESULT CRun_Player_Begin::Initialize()
{
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.f;


    return S_OK;
}

void CRun_Player_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{


    bBeginChange = false;

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CRun_Player_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_RUN_PLAYER_R;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::LSHIFT)
        return STATE_SPRINT_BEGIN_PLAYER;

    if (!bBeginChange)
    {
        if (KEY(W, HOLD))
        {
            // Key(CTRL + W + A)
            if (KEY(A, HOLD))
            {
                // 예외처리
                if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NW])
                {
                    Changing_Location(STATE_DIRECTION_NW, pAnimator);
                }
            }

            // Key(CTRL + W + D)
            else if (KEY(D, HOLD))
            {
                // 예외처리
                if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NE])
                {
                    Changing_Location(STATE_DIRECTION_NE, pAnimator);
                }
            }

            // Key(CTRL + W)
            else
            {
                // 예외처리
                if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_N])
                {
                    Changing_Location(STATE_DIRECTION_N, pAnimator);
                }
            }


        }

        // Key(CTRL + S)
        else if (KEY(S, HOLD))
        {

            // Key(CTRL + S + A)
            if (KEY(A, HOLD))
            {
                // 예외처리
                if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SW])
                {
                    Changing_Location(STATE_DIRECTION_SW, pAnimator);
                }
            }

            // Key(CTRL + S + D)
            else if (KEY(D, HOLD))
            {
                // 예외처리
                if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SE])
                {
                    Changing_Location(STATE_DIRECTION_SE, pAnimator);
                }
            }

            // Key(CTRL + S)
            else
            {
                // 예외처리
                if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_S])
                {
                    Changing_Location(STATE_DIRECTION_S, pAnimator);
                }
            }
        }

        // Key(CTRL + A)
        else if (KEY(A, HOLD))
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_W])
            {
                Changing_Location(STATE_DIRECTION_W, pAnimator);
            }
        }

        // Key(CTRL + D)
        else if (KEY(D, HOLD))
        {
            // 예외처리
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_E])
            {
                Changing_Location(STATE_DIRECTION_E, pAnimator);
            }
        }
    }
    

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Player_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Player_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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


void CRun_Player_Begin::Changing_Location(_uint iDirection, CAnimator* pAnimator)
{
    m_iAnimIndex = m_VecDirectionAnimIndex[iDirection];

    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);

    bBeginChange = true;
}