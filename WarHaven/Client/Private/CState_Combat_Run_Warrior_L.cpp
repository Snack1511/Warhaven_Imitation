#include "stdafx.h"
#include "CState_Combat_Run_Warrior_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Run_Warrior_L::CState_Combat_Run_Warrior_L()
{
}

CState_Combat_Run_Warrior_L::~CState_Combat_Run_Warrior_L()
{
}

CState_Combat_Run_Warrior_L* CState_Combat_Run_Warrior_L::Create()
{
    CState_Combat_Run_Warrior_L* pInstance = new CState_Combat_Run_Warrior_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Run_Warrior_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Run_Warrior_L::Initialize()
{
    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 19;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEFAULT_WARRIOR_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;

    m_iAINextState = AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L;
    m_eJumpFallStateType = AI_STATE_COMMON_LAND_WARRIOR_L;

    return __super::Initialize();
}

void CState_Combat_Run_Warrior_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Run_Warrior_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
    {

        switch (m_iRand)
        {
        case 0:

            return AI_STATE_COMBAT_HORIZONTALUP_WARRIOR_L;

        case 1:
        case 2:

            return AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L;

        case 3:

            return AI_STATE_COMBAT_HORIZONTALDOWN_WARRIOR_L;

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

void CState_Combat_Run_Warrior_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Run_Warrior_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
