#include "CState_Common_Land_Archer_L.h"
#include "CState.h"

#include "UsefulHeaders.h"

CState_Common_Land_Archer_L::CState_Common_Land_Archer_L()
{
}

CState_Common_Land_Archer_L::~CState_Common_Land_Archer_L()
{
}

CState_Common_Land_Archer_L* CState_Common_Land_Archer_L::Create()
{
    CState_Common_Land_Archer_L* pInstance = new CState_Common_Land_Archer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Land_Archer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Land_Archer_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 8;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_LAND_ARCHER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    return __super::Initialize();
}

void CState_Common_Land_Archer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Land_Archer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Land_Archer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CState_Common_Land_Archer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
