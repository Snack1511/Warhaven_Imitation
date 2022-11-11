#include "stdafx.h"
#include "CWarrior_Oxen.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen::CWarrior_Oxen()
{
}

CWarrior_Oxen::~CWarrior_Oxen()
{
}

CWarrior_Oxen* CWarrior_Oxen::Create()
{
    CWarrior_Oxen* pInstance = new CWarrior_Oxen();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 24;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PLAYER_SKILL1;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 84;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    m_iCancelAnimIndex = 22;

    m_iPlayAnimIndex = 23;

    m_iLoopAnimIndex = 25;
    m_iBeginAnimIndex = 24;

    m_iEndFinishedFrame = 3;

    return S_OK;
}

void CWarrior_Oxen::Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex)
{
    pOwner->TurnOn_TrailEffect(true);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, iPreAnimIndex);

    // CUser::Get_Instance()->Turn_BloodOverLay();
}

STATE_TYPE CWarrior_Oxen::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 9 : 상단 베기(R Attack)
    // 23 : 황소 베기(SKill)
    //Create_SwordEffect(pOwner);
    

    // 황소베기 실행중
    if (m_iAnimIndex == m_iPlayAnimIndex)
    {
        m_iStateChangeKeyFrame = 87;

        if (pAnimator->Is_CurAnimFinished())
            return STATE_IDLE_PLAYER_L;
    }

    // 황소 베기 캔슬
    if (m_iAnimIndex == m_iCancelAnimIndex)
    {
        m_iStateChangeKeyFrame = 20;

        if (pAnimator->Is_CurAnimFinished())
            return STATE_IDLE_PLAYER_R;
    }

    // 황소 베기 전
    if (m_iAnimIndex == m_iBeginAnimIndex)
    {
        if (KEY(Q, NONE))
        {
            Play_Skill(m_iPlayAnimIndex, 0.1f, pOwner, pAnimator);
        }

        else if(pAnimator->Is_CurAnimFinished())
        {
            m_fInterPolationTime = 0.f;

            Change_Animation(m_iAnimIndex, m_iLoopAnimIndex, pOwner, pAnimator);
        }
    }

    // 황소 베기 Loop
    if (m_iAnimIndex == m_iLoopAnimIndex)
    {
        if (KEY(Q, NONE))
        {
            Play_Skill(m_iPlayAnimIndex, 0.1f, pOwner, pAnimator);
        }
        

        if (pAnimator->Is_CurAnimFinished())
        {
            ++m_iCurFinishedFrame;

            if (m_iCurFinishedFrame == m_iEndFinishedFrame)
            {
                m_iCurFinishedFrame = 0;
                Change_Animation(m_iAnimIndex, m_iCancelAnimIndex, pOwner, pAnimator);
                return __super::Tick(pOwner, pAnimator);
                
            }

            m_fInterPolationTime = 0.f;

            Change_Animation(m_iAnimIndex, m_iLoopAnimIndex, pOwner, pAnimator);
        }

        
    }

    // 황소 베기와 오른쪽 공격 같은 것들을 넣어줘야함.
    // 기본적으로 오른손 파지를 전제로 하기 떄문에 Base_L 만 바꿔주면됨.


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CWarrior_Oxen::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Oxen 으로 오는 조건
    1.  Q 을 이용해 공격한다.
        */

    if (KEY(Q, TAP))
        return m_eStateType;

    return STATE_END;
}

void CWarrior_Oxen::Play_Skill(_uint iChangeIndex, _float fInterPolationTime, CUnit* pOwner, CAnimator* pAnimator)
{
    m_fInterPolationTime = fInterPolationTime;

    Change_Animation(m_iAnimIndex, m_iPlayAnimIndex, pOwner, pAnimator);
}


