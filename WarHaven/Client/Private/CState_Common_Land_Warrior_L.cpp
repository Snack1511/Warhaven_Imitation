#include "CState_Common_Land_Warrior_L.h"
#include "CState.h"

#include "UsefulHeaders.h"

CState_Common_Land_Warrior_L::CState_Common_Land_Warrior_L()
{
}

CState_Common_Land_Warrior_L::~CState_Common_Land_Warrior_L()
{
}

CState_Common_Land_Warrior_L* CState_Common_Land_Warrior_L::Create()
{
    CState_Common_Land_Warrior_L* pInstance = new CState_Common_Land_Warrior_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Land_Warrior_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Land_Warrior_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_LAND_WARRIOR_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iAINextState = AI_STATE_COMBAT_DEFAULT_WARRIOR_L;

    return S_OK;
}

void CState_Common_Land_Warrior_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Land_Warrior_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Land_Warrior_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CState_Common_Land_Warrior_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
