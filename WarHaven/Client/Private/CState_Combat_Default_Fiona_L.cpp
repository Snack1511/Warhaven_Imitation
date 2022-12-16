#include "stdafx.h"
#include "CState_Combat_Default_Fiona_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CPlayer.h"

#include "CUser.h"

CState_Combat_Default_Fiona_L::CState_Combat_Default_Fiona_L()
{
}

CState_Combat_Default_Fiona_L::~CState_Combat_Default_Fiona_L()
{
}

CState_Combat_Default_Fiona_L* CState_Combat_Default_Fiona_L::Create()
{
    CState_Combat_Default_Fiona_L* pInstance = new CState_Combat_Default_Fiona_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Default_Fiona_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Default_Fiona_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 20;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEFAULT_FIONA_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

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

    m_iAINextState = AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L;

    return S_OK;
}

void CState_Combat_Default_Fiona_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Fiona_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
    {

        switch (m_iRand)
        {
        case 0:
        case 1:


            return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L;

        case 2:
        case 3:

            return AI_STATE_COMBAT_VERTICALCUT_FIONA;

        case 4:
        case 5:


            if (m_ePreStateType != AI_STATE_COMBAT_GUARDBEGIN_FIONA)
                return AI_STATE_COMBAT_GUARDBEGIN_FIONA;
            else
                return m_eStateType;

        case 6:
        case 7:

            if (pOwner->Can_Use(CUnit::SKILL1))
                return AI_STATE_COMBAT_COUNTER_FIONA;

            else if(pOwner->Can_Use(CUnit::SKILL3))
                return AI_STATE_COMBAT_SPINATTACK_FIONA;

            else if(pOwner->Get_OwnerPlayer()->Get_Gauge() > 15.f)
                return AI_STATE_COMBAT_SHIELDATTACK_FIONA;

            else
                return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L;

        default:
            break;
        }
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Fiona_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Fiona_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
