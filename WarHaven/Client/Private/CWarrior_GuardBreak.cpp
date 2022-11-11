#include "stdafx.h"
#include "CWarrior_GuardBreak.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

//CColorController::COLORDESC tColorDesc;

CWarrior_GuardBreak::CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak::~CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak* CWarrior_GuardBreak::Create()
{
    CWarrior_GuardBreak* pInstance = new CWarrior_GuardBreak();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_GuardBreak");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_GuardBreak::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PLAYER_SKILL2;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 77;
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);
    //ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

    //tColorDesc.eFadeStyle = CColorController::KEYFRAME;
    //tColorDesc.fFadeInStartTime = 1.f;
    //tColorDesc.fFadeInTime = 1.f;
    //tColorDesc.fFadeOutStartTime = 1.f;
    //tColorDesc.fFadeOutTime = 1.f;
    //tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 1.f);
    //tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    //tColorDesc.iStartKeyFrame = 10;
    //tColorDesc.iEndKeyFrame = 30;

    return S_OK;
}

void CWarrior_GuardBreak::Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, iPreAnimIndex);
}

STATE_TYPE CWarrior_GuardBreak::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 9 : 상단 베기(R Attack)
    // 23 : 황소 베기(SKill)
    //Create_SwordEffect(pOwner);

    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PLAYER_R;


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_GuardBreak::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWarrior_GuardBreak::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  스킬버튼 을 이용해 공격한다.
    */

    if (KEY(T, TAP))
        return m_eStateType;


    return STATE_END;
}
