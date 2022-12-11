#include "stdafx.h"
#include "CState_Combat_Run_Warrior_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Run_Warrior_R::CState_Combat_Run_Warrior_R()
{
}

CState_Combat_Run_Warrior_R::~CState_Combat_Run_Warrior_R()
{
}

CState_Combat_Run_Warrior_R* CState_Combat_Run_Warrior_R::Create()
{
    CState_Combat_Run_Warrior_R* pInstance = new CState_Combat_Run_Warrior_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Run_Warrior_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Run_Warrior_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 27;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEAFULT_WARRIOR_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;


    m_fAnimSpeed = 2.5f;


	// 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 26;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 27;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 28;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 29;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 30;

    m_iAINextState = AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R;

    return S_OK;
}

void CState_Combat_Run_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Run_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
    {
        switch (m_iRand)
        {
        case 0:
        case 1:
        case 2:
        case 3:

            return m_iAINextState;

        case 4:
        case 5:


            if (m_ePreStateType != AI_STATE_COMBAT_GUARDBEGIN_WARRIOR)
                return AI_STATE_COMBAT_GUARDBEGIN_WARRIOR;
            else
                return m_eStateType;


        case 7:
        case 6:

            if (pOwner->Can_Use(CUnit::SKILL1))
                return AI_STATE_COMBAT_GUARDBREAK_WARRIOR;

            else
                return AI_STATE_COMBAT_GUARDBEGIN_WARRIOR;

        default:
            break;
        }
    }



    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Run_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Run_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
