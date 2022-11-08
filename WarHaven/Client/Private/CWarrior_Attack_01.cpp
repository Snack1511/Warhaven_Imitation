#include "stdafx.h"
#include "CWarrior_Attack_01.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWarrior_Attack_01::CWarrior_Attack_01()
{
}

CWarrior_Attack_01::~CWarrior_Attack_01()
{
}

CWarrior_Attack_01* CWarrior_Attack_01::Create()
{
    CWarrior_Attack_01* pInstance = new CWarrior_Attack_01();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_01");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_01::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 50;
    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CWarrior_Attack_01::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

    pAnimator->Set_CurAnimIndex(m_eAnimType, 0);
}

STATE_TYPE CWarrior_Attack_01::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_01::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWarrior_Attack_01::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  LBuutton 을 이용해 공격한다.
    2.  CTRL LButton 을 이용해 내려찍는다.
    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}
