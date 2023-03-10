#include "stdafx.h"
#include "CBounce_Archer_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_Archer_R::CBounce_Archer_R()
{
}

CBounce_Archer_R::~CBounce_Archer_R()
{
}

CBounce_Archer_R* CBounce_Archer_R::Create()
{
    CBounce_Archer_R* pInstance = new CBounce_Archer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_Archer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_Archer_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fAnimSpeed = 2.5f;

    m_iStateChangeKeyFrame = 60;

    m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
    m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_ARCHER_R);
    m_vecAdjState.push_back(STATE_JUMP_ARCHER_R);

    return S_OK;
}

void CBounce_Archer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_ATTACK_SHOOT_SNIPING_ARCHER ||
        ePrevType == STATE_ATTACK_SHOOT_POISION_ARCHER ||
        ePrevType == STATE_ATTACK_SHOOT_ARCHER)
    {
        m_ePreStateType = ePrevType;
        m_bAttackTrigger = true;
        return;
    }
    else
        m_ePreStateType = STATE_END;
    


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

    Play_Sound(L"Effect_Bounce");
}

STATE_TYPE CBounce_Archer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_ARCHER_R;

    if (m_bAttackTrigger)
        return m_ePreStateType;

    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Archer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_Archer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}