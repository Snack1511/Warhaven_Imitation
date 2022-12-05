#include "stdafx.h"
#include "CJump_Paladin_Fall_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Paladin_Fall_L::CJump_Paladin_Fall_L()
{
}

CJump_Paladin_Fall_L::~CJump_Paladin_Fall_L()
{
}

CJump_Paladin_Fall_L* CJump_Paladin_Fall_L::Create()
{
    CJump_Paladin_Fall_L* pInstance = new CJump_Paladin_Fall_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Paladin_Fall_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_Paladin_Fall_L::Initialize()
{



    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_PALADIN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.15f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;


    m_vecAdjState.push_back(STATE_JUMP_LAND_PALADIN_L);


    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);
    //m_vecAdjState.push_back(STATE_VERTICALATTACK_PALADIN_L);


    return S_OK;
}

void CJump_Paladin_Fall_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_JUMP_PALADIN_L)
        m_fInterPolationTime = 0.05f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Paladin_Fall_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return STATE_JUMP_LAND_PALADIN_L;

   return __super::Tick(pOwner, pAnimator);
}

void CJump_Paladin_Fall_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_Paladin_Fall_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN가 Walk로 오는 조건
    1. Jump 를 끝내면.
    */

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
