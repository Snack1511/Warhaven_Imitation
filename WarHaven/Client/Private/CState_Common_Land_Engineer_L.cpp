#include "CState_Common_Land_Engineer_L.h"
#include "CState.h"

#include "UsefulHeaders.h"

CState_Common_Land_Engineer_L::CState_Common_Land_Engineer_L()
{
}

CState_Common_Land_Engineer_L::~CState_Common_Land_Engineer_L()
{
}

CState_Common_Land_Engineer_L* CState_Common_Land_Engineer_L::Create()
{
    CState_Common_Land_Engineer_L* pInstance = new CState_Common_Land_Engineer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Land_Engineer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Land_Engineer_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 10;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_LAND_ENGINEER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    return S_OK;
}

void CState_Common_Land_Engineer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Land_Engineer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Land_Engineer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CState_Common_Land_Engineer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
