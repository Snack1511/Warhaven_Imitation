#include "stdafx.h"
#include "CSprint_Jump_Fall_Priest.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Jump_Fall_Priest::CSprint_Jump_Fall_Priest()
{
}

CSprint_Jump_Fall_Priest::~CSprint_Jump_Fall_Priest()
{
}

CSprint_Jump_Fall_Priest* CSprint_Jump_Fall_Priest::Create()
{
    CSprint_Jump_Fall_Priest* pInstance = new CSprint_Jump_Fall_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Jump_Fall_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Jump_Fall_Priest::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 45;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_JUMPFALL_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_GLIDING);
    m_vecAdjState.push_back(STATE_JUMP_LAND_PRIEST);
    m_vecAdjState.push_back(STATE_PROJECTILECATCH_BEGIN_PRIEST);
    m_vecAdjState.push_back(STATE_AIRDASH_PRIEST);
    m_vecAdjState.push_back(STATE_WINDATTACK_PRIEST);
    m_vecAdjState.push_back(STATE_CURE_BEGIN_PRIEST);
    m_vecAdjState.push_back(STATE_ATTACK_STING_PRIEST);

    return S_OK;
}

void CSprint_Jump_Fall_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSprint_Jump_Fall_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
    {
        STATE_TYPE eSprintEndState = pOwner->Get_SprintEndState();
        return eSprintEndState;
    }


    Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Jump_Fall_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSprint_Jump_Fall_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PRIEST가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 점프한다.
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


   
    return STATE_END;
}
