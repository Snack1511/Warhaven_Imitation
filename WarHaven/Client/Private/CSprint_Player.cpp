#include "stdafx.h"
#include "CSprint_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Player::CSprint_Player()
{
}

CSprint_Player::~CSprint_Player()
{
}

CSprint_Player* CSprint_Player::Create()
{
    CSprint_Player* pInstance = new CSprint_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Player::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 55;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
//    m_eStateType = STATE_SPRINT_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CSprint_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_eAnimType = ANIM_BASE_R;
    m_iAnimIndex = 55;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
        
    // 만약 쉬프트키 나 W 를 해제했을 시
    if (KEY(LSHIFT, NONE) || KEY(W, NONE))
    {
        // Sprint_End 호출
        if (m_iAnimIndex != 56)
        {
            m_iAnimIndex = 56;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
        }

        if (m_iAnimIndex == 56 && pAnimator->Is_CurAnimFinished())
        {

          //  return STATE_IDLE_PLAYER;
        }
    }

    // Sprint_End가 프레임이 끝났을 시 
    if (m_iAnimIndex == 56 && pAnimator->Is_CurAnimFinished())
    {
        //return STATE_IDLE_PLAYER;
    }


    // Sprint Begin의 프레임이 끝났을 시 Sprint 호출
    if (m_iAnimIndex == 55 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 60;
     
        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    }


    // Sprint_Jump 점프가 끝나면 FallLoop 으로 떨어지는 자세를 만듦
    if (m_iAnimIndex == 58 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 57;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    }

    if (m_iAnimIndex == 57 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 56;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    }


    // Sprint 상태에서 Space 를 눌르면 Sprint_Jump 상태로 넘어감
    if (KEY(SPACE, TAP))
    {
        if (m_iAnimIndex != 58)
        {
            m_iAnimIndex = 58;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
        }
    }

    // 왼쪽 버튼을 누를 시 (스프린트 공격 시 )
    if (KEY(LBUTTON, TAP))
    {
        // SprintAttack_Begin
        if (m_iAnimIndex != 20)
        {
            m_iAnimIndex = 20;
            m_eAnimType = ANIM_ATTACK;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);

        }
    }

    // SprintAttack_Begin 이 끝날 시 
    else if (m_iAnimIndex == 20 && pAnimator->Is_CurAnimFinished())
    {
        m_iAnimIndex = 21;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
    }

    else if (m_iAnimIndex == 21 && pAnimator->Is_CurAnimFinished())
    {
       // return STATE_IDLE_PLAYER;
    }

    if (m_iAnimIndex == 21 && pAnimator->Get_CurAnimFrame() > 20)
    {
        int a = 0;
    }


    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSprint_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 움직인다.
    */


    if (KEY(LSHIFT, HOLD))
    {
        if(KEY(W, HOLD))
         return m_eStateType;

      
            return m_eStateType;
            

    }

    if (m_iAnimIndex == 56 ||
        m_iAnimIndex == 57 ||
        m_iAnimIndex == 58 || 
        m_iAnimIndex == 60)
        return m_eStateType;
   
    return STATE_END;
}
