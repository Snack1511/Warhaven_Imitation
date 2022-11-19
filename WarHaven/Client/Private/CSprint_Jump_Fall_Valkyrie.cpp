#include "stdafx.h"
#include "CSprint_Jump_Fall_Valkyrie.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Jump_Fall_Valkyrie::CSprint_Jump_Fall_Valkyrie()
{
}

CSprint_Jump_Fall_Valkyrie::~CSprint_Jump_Fall_Valkyrie()
{
}

CSprint_Jump_Fall_Valkyrie* CSprint_Jump_Fall_Valkyrie::Create()
{
    CSprint_Jump_Fall_Valkyrie* pInstance = new CSprint_Jump_Fall_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Jump_Fall_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Jump_Fall_Valkyrie::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 47;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_JUMPFALL_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;


    m_vecAdjState.push_back(STATE_JUMP_LAND_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_SPRINT_END_VALKYRIE);


    return S_OK;
}

void CSprint_Jump_Fall_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSprint_Jump_Fall_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Jump_Fall_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSprint_Jump_Fall_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 점프한다.
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


   
    return STATE_END;
}
