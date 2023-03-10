#include "stdafx.h"
#include "CJump_SpearMan_Fall_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_SpearMan_Fall_L::CJump_SpearMan_Fall_L()
{
}

CJump_SpearMan_Fall_L::~CJump_SpearMan_Fall_L()
{
}

CJump_SpearMan_Fall_L* CJump_SpearMan_Fall_L::Create()
{
    CJump_SpearMan_Fall_L* pInstance = new CJump_SpearMan_Fall_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_SpearMan_Fall_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_SpearMan_Fall_L::Initialize()
{
    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_SPEARMAN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.f;

    m_fAnimSpeed = 2.5f;


    m_vecAdjState.push_back(STATE_JUMP_LAND_SPEARMAN_L);

    return S_OK;
}

void CJump_SpearMan_Fall_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_SpearMan_Fall_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return STATE_JUMP_LAND_SPEARMAN_L;

   return __super::Tick(pOwner, pAnimator);
}

void CJump_SpearMan_Fall_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_SpearMan_Fall_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN가 Walk로 오는 조건
    1. Jump 를 끝내면.
    */

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
