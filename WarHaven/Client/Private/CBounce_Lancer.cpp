#include "stdafx.h"
#include "CBounce_Lancer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_Lancer::CBounce_Lancer()
{
}

CBounce_Lancer::~CBounce_Lancer()
{
}

CBounce_Lancer* CBounce_Lancer::Create()
{
    CBounce_Lancer* pInstance = new CBounce_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_Lancer::Initialize()
{

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fAnimSpeed = 2.5f;

    m_iStateChangeKeyFrame = 60;

    m_vecAdjState.push_back(STATE_IDLE_LANCER);
    m_vecAdjState.push_back(STATE_RUN_LANCER);
    m_vecAdjState.push_back(STATE_JUMP_LANCER);

    return S_OK;
}

void CBounce_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

    Play_Sound(L"Effect_Bounce");
}

STATE_TYPE CBounce_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_LANCER;

    if (m_bAttackTrigger)
        return m_ePreStateType;

    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}