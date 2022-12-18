#include "stdafx.h"
#include "CRevive_Player.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CRevive_Player::CRevive_Player()
{
}

CRevive_Player::~CRevive_Player()
{
}

CRevive_Player* CRevive_Player::Create()
{
    CRevive_Player* pInstance = new CRevive_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRevive_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRevive_Player::Initialize()
{
    // 상하체 분리.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 28;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_REVIVE_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_JUMP_PLAYER_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

    m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);



    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);


    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

    m_vecAdjState.push_back(STATE_CHANGE_PLAYER);


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.5f;


    return S_OK;
}

void CRevive_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->Get_RevivalPlayer()->Get_CurrentUnit()->Start_Reborn();


    _float4 vPos = pOwner->Get_RevivalPlayer()->Get_WorldPos();
    _float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vDir = vPos - vMyPos;
    pOwner->Get_Transform()->Set_LerpLook(vDir.Normalize(), 0.4f);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRevive_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    switch (m_eCurPhase)
    {
    case Client::CRevive_Player::BEGIN:

        if (pAnimator->Is_CurAnimFinished())
        {
            m_eCurPhase = LOOP;
            m_iAnimIndex = 29;
            __super::Enter(pOwner, pAnimator, m_eStateType);
        }

        break;
    case Client::CRevive_Player::LOOP:
        if (m_fTimeAcc >= 4.f)
        {
            m_eCurPhase = PHASE_END;
            m_iAnimIndex = 30;
            __super::Enter(pOwner, pAnimator, m_eStateType);
        }

        break;
    case Client::CRevive_Player::PHASE_END:
        if (pAnimator->Is_CurAnimFinished())
        {
            STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
            return eDefaultState;
        }
        break;
    default:
        break;
    }





    return __super::Tick(pOwner, pAnimator);
}

void CRevive_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRevive_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
    if (pOwner->Get_RevivalPlayer())
    {
        if (KEY(F, TAP))
        {
            return m_eStateType;
        }
    }

    return STATE_END;
}
