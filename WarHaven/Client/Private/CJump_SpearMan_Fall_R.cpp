#include "stdafx.h"
#include "CJump_SpearMan_Fall_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
CJump_SpearMan_Fall_R::CJump_SpearMan_Fall_R()
{
}

CJump_SpearMan_Fall_R::~CJump_SpearMan_Fall_R()
{
}


HRESULT CJump_SpearMan_Fall_R::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 10;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_SPEARMAN_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.f;

    m_fAnimSpeed = 2.5f;


    m_vecAdjState.push_back(STATE_JUMP_LAND_SPEARMAN_R);


    return S_OK;
}

CJump_SpearMan_Fall_R* CJump_SpearMan_Fall_R::Create()
{
    CJump_SpearMan_Fall_R* pInstance = new CJump_SpearMan_Fall_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_SpearMan_Fall_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CJump_SpearMan_Fall_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
 
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_SpearMan_Fall_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 땅에 닿았을 시 
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return STATE_JUMP_LAND_SPEARMAN_R;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_SpearMan_Fall_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_SpearMan_Fall_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN가 Walk로 오는 조건
    1. Jump 를 끝내면.
    */

    if(pAnimator->Is_CurAnimFinished())
        return m_eStateType;
  

    return STATE_END;
}
