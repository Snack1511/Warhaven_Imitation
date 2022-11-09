#include "stdafx.h"
#include "CIdle_Player.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CIdle_Player::CIdle_Player()
{
}

CIdle_Player::~CIdle_Player()
{
}

CIdle_Player* CIdle_Player::Create()
{
    CIdle_Player* pInstance = new CIdle_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Player::Initialize()
{
    // 상하체 분리.
    // 


    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 11;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    
    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    //m_vecAdjState.push_back(STATE_JUMP);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_SPRINT_PLAYER);


    return S_OK;
}

void CIdle_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    // R_Idle L_Idle Setting
    if (m_eAnimType == ANIM_BASE_R)
    {
        m_iAnimIndex = 11;
    }
    else if (m_eAnimType == ANIM_BASE_L)
    {
        m_iAnimIndex = 3;
    }


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CIdle_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        // 만약 RF to L 상태라면(오른쪽 정면 -> 왼쪽)
        if (m_iAnimIndex == 53)
        {
            // RF_Idle 상태로 바꾼다.
            m_eAnimType = ANIM_BASE_L;
            m_iAnimIndex = 3;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }

        // 만약 L to R 상태라면(왼쪽 -> 오른쪽)
        if (m_iAnimIndex == 27)
        {
            // R_Idle 상태로 바꾼다.
            m_eAnimType = ANIM_BASE_R;
            m_iAnimIndex = 11;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.2f);
        }
    }
        
    if (KEY(R, TAP))
    {
        // 만약 오른쪽으로 들고있으면
        if (m_eAnimType == ANIM_BASE_R)
        {
            // Idle 상태이며 && R to L 상태가 들어오지 않았다면
            if (m_iAnimIndex == 11 && m_iAnimIndex != 53)
            {
                m_iAnimIndex = 53;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.2f);
            }
        }
    }

    else if (KEY(E, TAP))
    {
        if (m_eAnimType == ANIM_BASE_L)
        {
            // Idle 상태이며 && // 만약 L to R 상태가 들어오지 않았다면
            if (m_iAnimIndex == 3 && m_iAnimIndex != 27)
            {
                m_iAnimIndex = 27;

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
            }
        }

    }

    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CIdle_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건 
    1. 현재 진행중인 애니메이션이 끝났을 때
    */


    if (KEY(SPACE, NONE))
    {
        if (pAnimator->Is_CurAnimFinished())
            return m_eStateType;
    }
  


    return STATE_END;
}

void CIdle_Player::Switch_Right_And_Left(_uint iAnimType, _uint iAnimIndex, _uint iChangeAnimIndex, CAnimator* pAnimator)
{
    if (m_eAnimType == iAnimType == m_iAnimIndex == iAnimIndex)
    {
        m_iAnimIndex = iChangeAnimIndex;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    }
}
