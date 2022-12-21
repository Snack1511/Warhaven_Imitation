#include "stdafx.h"
#include "CWarrior_Oxen_Loop.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen_Loop::CWarrior_Oxen_Loop()
{
}

CWarrior_Oxen_Loop::~CWarrior_Oxen_Loop()
{
}

CWarrior_Oxen_Loop* CWarrior_Oxen_Loop::Create()
{
    CWarrior_Oxen_Loop* pInstance = new CWarrior_Oxen_Loop();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen_Loop");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen_Loop::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 25;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_LOOP;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 3.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 2;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_LOOPATTACK);

    m_eBounceState = STATE_BOUNCE_PLAYER_R;

    m_iEndFinishedFrame = 3;

    return S_OK;
}

void CWarrior_Oxen_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->TurnOn_TrailEffect(true);

    _float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Crow_Charge", pOwner, vPos);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Oxen_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
    {
        ++m_iCurFinishedFrame;

        if (m_iCurFinishedFrame == m_iEndFinishedFrame)
        {
            m_iCurFinishedFrame = 0;
            return STATE_WARRIOR_OXEN_END;
        }
    }

    // 황소 베기와 오른쪽 공격 같은 것들을 넣어줘야함.
    // 기본적으로 오른손 파지를 전제로 하기 떄문에 Base_L 만 바꿔주면됨.


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CWarrior_Oxen_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Oxen 으로 오는 조건
    1.  Q 을 이용해 공격한다.
        */

    return STATE_END;
}