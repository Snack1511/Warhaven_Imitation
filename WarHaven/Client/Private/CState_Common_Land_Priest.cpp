#include "stdafx.h"
#include "CState_Common_Land_Priest.h"


#include "UsefulHeaders.h"


CState_Common_Land_Priest::CState_Common_Land_Priest()
{
}

CState_Common_Land_Priest::~CState_Common_Land_Priest()
{
}

CState_Common_Land_Priest* CState_Common_Land_Priest::Create()
{
    CState_Common_Land_Priest* pInstance = new CState_Common_Land_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Land_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Land_Priest::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 16;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_LAND_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    return __super::Initialize();
}

void CState_Common_Land_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Land_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Land_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Land_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
