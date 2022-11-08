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
    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);

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

    // 만약 WASD 를 눌렀다면
    if (KEY(W, HOLD) ||
        KEY(A, HOLD) ||
        KEY(S, HOLD) ||
        KEY(D, HOLD))
    {
     
        // 만약 CTRL 를 눌렀다면
        if (KEY(Z, HOLD))
        {
            // 만약 W 를 눌렀다면(정면)
            if (KEY(W, HOLD))
            {
                // A 를 눌렀다면(정면왼쪽)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 33;
                }

                // A 를 눌렀다면(정면오른쪽)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 44;
                }
            }

            // 만약 S 를 눌렀다면(후방)
            else if (KEY(S, HOLD))
            {
                // 만약 A 를 눌렀다면(후방왼쪽)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 33;
                }

                // 만약 A 를 눌렀다면(후방오른쪽)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 44;
                }
            }

            
            // A를 눌렀다면(왼쪽)
            else if(KEY(A, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }

            // D를 눌렀다면(오른쪽)
            else if (KEY(D, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }
        }
        else
        {
            // 만약 W 를 눌렀다면(정면)
            if (KEY(W, HOLD))
            {
                // A 를 눌렀다면(정면왼쪽)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 22;
                }

                // A 를 눌렀다면(정면오른쪽)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 26;
                }
            }

            // 만약 S 를 눌렀다면(후방)
            else if (KEY(S, HOLD))
            {
                // 만약 A 를 눌렀다면(후방왼쪽)
                if (KEY(A, HOLD))
                {
                    m_eAnimType = ANIM_BASE_L;
                    m_iAnimIndex = 33;
                }

                // 만약 A 를 눌렀다면(후방오른쪽)
                if (KEY(D, HOLD))
                {
                    m_eAnimType = ANIM_BASE_R;
                    m_iAnimIndex = 44;
                }
            }


            // A를 눌렀다면(왼쪽)
            else if (KEY(A, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }

            // D를 눌렀다면(오른쪽)
            else if (KEY(D, HOLD))
            {
                m_eAnimType = ANIM_BASE_R;
                m_iAnimIndex = 30;
            }
        }

        return m_eStateType;
    }




    return STATE_END;
}
