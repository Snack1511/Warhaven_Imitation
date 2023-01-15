#include "stdafx.h"
#include "CState_Combat_Default_Archer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CPlayer.h"

#include "CUser.h"

CState_Combat_Default_Archer_L::CState_Combat_Default_Archer_L()
{
}

CState_Combat_Default_Archer_L::~CState_Combat_Default_Archer_L()
{
}

CState_Combat_Default_Archer_L* CState_Combat_Default_Archer_L::Create()
{
    CState_Combat_Default_Archer_L* pInstance = new CState_Combat_Default_Archer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Default_Archer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Default_Archer_L::Initialize()
{
    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 19;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEFAULT_ARCHER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 21;

    m_eJumpFallStateType = AI_STATE_COMMON_LAND_ARCHER_L;

    return __super::Initialize();
}

void CState_Combat_Default_Archer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Archer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Archer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Archer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
