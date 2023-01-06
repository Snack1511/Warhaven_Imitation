#include "stdafx.h"
#include "CState_Combat_Default_Priest.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CPlayer.h"

#include "CUser.h"

CState_Combat_Default_Priest::CState_Combat_Default_Priest()
{
}

CState_Combat_Default_Priest::~CState_Combat_Default_Priest()
{
}

CState_Combat_Default_Priest* CState_Combat_Default_Priest::Create()
{
    CState_Combat_Default_Priest* pInstance = new CState_Combat_Default_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Default_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Default_Priest::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 27;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEFAULT_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)
  
    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;
    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 10.f;

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 24;

    m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    return __super::Initialize();
}

void CState_Combat_Default_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == m_eStateType)
        m_fInterPolationTime = 0.f;

    m_iRand = random(0, 7);
    m_iDirectionRand = random(0, 7);

    Set_Direction_Back_AI(m_iDirectionRand);
    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
    m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

    m_iStateChangeKeyFrame = 25;

    m_fAIMyLength = 2.2f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    STATE_TYPE eAttackState = Random_State(pOwner, pAnimator);

    if (eAttackState != STATE_END)
        return eAttackState;

    _uint iFrame = pAnimator->Get_CurAnimFrame() + 1;

    switch (m_iDirectionRand)
    {
    case 0:
    case 1:
    case 2:

        if (iFrame == m_iStateChangeKeyFrame)
            return m_eStateType;

        break;

    case 3:
    case 4:
    case 5:
    case 6:

        if (iFrame == _uint(_float(m_iStateChangeKeyFrame) * 1.5f))
            return m_eStateType;

        break;



    case 7:

        if (pOwner->Can_Use(CUnit::SKILL3))
            return AI_STATE_COMBAT_CURE_BEGIN_PRIEST;

        else if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
            return m_eStateType;

    default:
        break;
    }


    DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Priest::Random_State(CUnit* pOwner, CAnimator* pAnimator)
{
    if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
    {

        switch (m_iRand)
        {
        case 0:
        case 1:
        case 2:
        case 3:

            if (pOwner->Can_Use(CUnit::SKILL1))
                return AI_STATE_COMBAT_AIRDASH_PRIEST;
            else
                return AI_STATE_COMBAT_STINGATTACK_PRIEST;

        
        case 4:
        case 5:
        case 6:
        case 7:

            if (pOwner->Can_Use(CUnit::SKILL2))
                return AI_STATE_COMBAT_AIRDASH_PRIEST;
            else
                return AI_STATE_COMBAT_STINGATTACK_PRIEST;

        default:
            break;
        }
    }

    return STATE_END;

}
