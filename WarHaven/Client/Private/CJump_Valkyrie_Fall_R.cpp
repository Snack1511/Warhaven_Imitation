#include "stdafx.h"
#include "CJump_Valkyrie_Fall_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
CJump_Valkyrie_Fall_R::CJump_Valkyrie_Fall_R()
{
}

CJump_Valkyrie_Fall_R::~CJump_Valkyrie_Fall_R()
{
}


HRESULT CJump_Valkyrie_Fall_R::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_VALKYRIE_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.15f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    //m_vecAdjState.push_back(STATE_IDLE_VALKYRIE);

    m_vecAdjState.push_back(STATE_JUMP_LAND_VALKYRIE_R);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
    //m_vecAdjState.push_back(STATE_VERTICALATTACK_VALKYRIE_R);



    return S_OK;
}

CJump_Valkyrie_Fall_R* CJump_Valkyrie_Fall_R::Create()
{
    CJump_Valkyrie_Fall_R* pInstance = new CJump_Valkyrie_Fall_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Valkyrie_Fall_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CJump_Valkyrie_Fall_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == STATE_JUMP_VALKYRIE_R)
        m_fInterPolationTime = 0.05f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Valkyrie_Fall_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 땅에 닿았을 시 
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return STATE_JUMP_LAND_VALKYRIE_R;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Valkyrie_Fall_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_Valkyrie_Fall_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE가 Walk로 오는 조건
    1. Jump 를 끝내면.
    */

    if(pAnimator->Is_CurAnimFinished())
        return m_eStateType;
  

    return STATE_END;
}
