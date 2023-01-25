#include "stdafx.h"
#include "CJump_Lancer_Fall.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
CJump_Lancer_Fall::CJump_Lancer_Fall()
{
}

CJump_Lancer_Fall::~CJump_Lancer_Fall()
{
}


HRESULT CJump_Lancer_Fall::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 28;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = FLT_MAX;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = FLT_MIN;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    //m_vecAdjState.push_back(STATE_IDLE_ARCHER);


    m_vecAdjState.push_back(STATE_JUMP_LAND_LANCER);

    return S_OK;
}

CJump_Lancer_Fall* CJump_Lancer_Fall::Create()
{
    CJump_Lancer_Fall* pInstance = new CJump_Lancer_Fall();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Lancer_Fall");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CJump_Lancer_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    //if (ePrevType == STATE_JUMP_LANCER)
    //    m_fInterPolationTime = 0.05f;

    CUser::Get_Instance()->Clear_KeyCommands();
    m_pOwner = pOwner;
    m_fTimeAcc = 0.f;
    m_bExecuted = false;
    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fAnimSpeed);

   //__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Lancer_Fall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);
    pOwner->Set_DirAsLook();

    if (!pOwner->Is_Air())
        return STATE_IDLE_LANCER;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Lancer_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_Lancer_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER가 Walk로 오는 조건
    1. Jump 를 끝내면.
    */

    if(pAnimator->Is_CurAnimFinished())
        return m_eStateType;
  

    return STATE_END;
}
