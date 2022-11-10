#include "stdafx.h"
#include "CJump_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Player::CJump_Player()
{
}

CJump_Player::~CJump_Player()
{
}

CJump_Player* CJump_Player::Create()
{
    CJump_Player* pInstance = new CJump_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Player::Initialize()
{



    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_IDLE_PLAYER);


    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CJump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */

    if (m_eAnimType == ANIM_BASE_R)
    {
        if (KEY(W, HOLD))
            m_iAnimIndex = 14;

        else if (KEY(S, HOLD))
            m_iAnimIndex = 15;

        else if (KEY(A, HOLD))
            m_iAnimIndex = 16;

        else if (KEY(D, HOLD))
            m_iAnimIndex = 13;

        else
            m_iAnimIndex = 12;
    }
  

    if (m_eAnimType == ANIM_BASE_L)
    {
        if (KEY(W, HOLD))
            m_iAnimIndex = 6;

        else if (KEY(S, HOLD))
            m_iAnimIndex = 7;

        else if (KEY(A, HOLD))
            m_iAnimIndex = 8;

        else if (KEY(D, HOLD))
            m_iAnimIndex = 5;

        else
            m_iAnimIndex = 4;
    }


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CJump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    // 프레임이 끝났을 시
    if (pAnimator->Is_CurAnimFinished())
    {
        // Base_R
        if (m_eAnimType == ANIM_BASE_R)
        {
            // 만약 점프 상태가 끝나고 점프 중일 시
            if (m_iAnimIndex != 10 && m_iAnimIndex != 17)
            {
                m_iAnimIndex = 10;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            // 점프 중에서 착지 할 시
            else if (m_iAnimIndex == 10)
            {
                m_iAnimIndex = 17;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            //착지가 끝날 시
            else
            {
                return STATE_IDLE_PLAYER;
            }
        }

        // Base_L
        if (m_eAnimType == ANIM_BASE_L)
        {
            // 만약 점프 상태가 끝나고 점프 중일 시
            if (m_iAnimIndex != 1 && m_iAnimIndex != 9)
            {
                m_iAnimIndex = 1;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            // 점프 중에서 착지 할 시
            else if (m_iAnimIndex == 1)
            {
                m_iAnimIndex = 9;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            }

            //착지가 끝날 시
            else
            {
                return STATE_IDLE_PLAYER;
            }
        }
        
    }
        
   
    return __super::Tick(pOwner, pAnimator);


    
}

void CJump_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 
    */

    // m_eStateType 이 End 에 가지 않으면 Enter 를 호출한다.

    // W 랑 A 를 누르면 왼쪽 앞으로 이동한다.
    // W 랑 D 를 누르면 왼쪽 옆으로 이동한다.

    // 만약 WASD 를 눌렀다면
    if (KEY(SPACE, TAP))
    {
        return m_eStateType;
    }
   
        //CTRL 로 바꾸셈.
      
      

   


    return STATE_END;
}
