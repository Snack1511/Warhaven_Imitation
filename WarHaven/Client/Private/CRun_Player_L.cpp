#include "stdafx.h"
#include "CRun_Player_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Player_L::CRun_Player_L()
{
}

CRun_Player_L::~CRun_Player_L()
{
}

CRun_Player_L* CRun_Player_L::Create()
{
    CRun_Player_L* pInstance = new CRun_Player_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Player_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Player_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_PLAYER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 9999;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
//    m_vecAdjState.push_back(STATE_SLIDE_PLAYER);
 //   m_vecAdjState.push_back(STATE_JUMP_PLAYER);
 //   m_vecAdjState.push_back(STATE_SPRINT_PLAYER);
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    // 15
    m_VecDirectionAnimIndex[STATE_DIRECTION_E] = 18;

    m_VecDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_VecDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_VecDirectionAnimIndex[STATE_DIRECTION_NW] = 21;

    m_VecDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_VecDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_VecDirectionAnimIndex[STATE_DIRECTION_SW] = 36;



    m_VecDirectionAnimIndex[STATE_DIRECTION_W] = 22;
    

    // ------------------------------------------------------

    // 234
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_E] = 10;
    
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_N] = 11;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NE] = 12;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NW] = 13;
   
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_S] = 14;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_SE] = 15;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_SW] = 16;
   
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_W] = 17;
   


    


    return S_OK;
}

void CRun_Player_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner의 Animator Set Idle로 */
    CColorController::COLORDESC m_tColorDesc;
    ZeroMemory(&m_tColorDesc, sizeof(CColorController::COLORDESC));

    m_tColorDesc.eFadeStyle = CColorController::TIME;
    m_tColorDesc.fFadeInStartTime = 1.f;
    m_tColorDesc.fFadeInTime = 1.f;
    m_tColorDesc.fFadeOutStartTime = 1.f;
    m_tColorDesc.fFadeOutTime = 1.f;

    m_tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 0.f);

    m_tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    m_tColorDesc.iStartKeyFrame = 3;
    m_tColorDesc.iEndKeyFrame = 6;


    GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);



    __super::Enter(pOwner, pAnimator, iPreAnimIndex);
}

STATE_TYPE CRun_Player_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    { 
        return STATE_IDLE_PLAYER_L;

    }

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Player_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Player_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. CTRL 을 누르지 않은 상태
    2. WASD 를 누른 상태
    */

    // m_eStateType 이 End 에 가지 않으면 Enter 를 호출한다.

    // W 랑 A 를 누르면 왼쪽 앞으로 이동한다.
    // W 랑 D 를 누르면 왼쪽 옆으로 이동한다.

    // 만약 WASD 를 눌렀다면

    // 걷기를 하지 않고
    if (KEY(CTRL, NONE))
    {
        // 황소 베기를 쓰지 않고
        if (KEY(Q, NONE))
        {
            // 점프를 하지 않고
            if (KEY(SPACE, NONE))
            {
                // 슬라이딩을 하지 않는다면? (나중에 깃발올리기, 소생 등으로 변경)
                if (KEY(J, NONE))
                {
                    if (KEY(W, HOLD) ||
                        KEY(A, HOLD) ||
                        KEY(S, HOLD) ||
                        KEY(D, HOLD))
                    {
                        return m_eStateType;
                    }
                }
            }
        }

    }
    

   


    return STATE_END;
}

//_bool CRun_Player_L::Change_Animation_Run(_uint iBeginAttackAnim, _uint iAttackAnim, CAnimator* pAnimator)
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
