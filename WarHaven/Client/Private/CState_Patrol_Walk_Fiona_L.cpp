#include "stdafx.h"
#include "CState_Patrol_Walk_Fiona_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Walk_Fiona_L::CState_Patrol_Walk_Fiona_L()
{
}

CState_Patrol_Walk_Fiona_L::~CState_Patrol_Walk_Fiona_L()
{
}

CState_Patrol_Walk_Fiona_L* CState_Patrol_Walk_Fiona_L::Create()
{
    CState_Patrol_Walk_Fiona_L* pInstance = new CState_Patrol_Walk_Fiona_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Walk_Fiona_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Walk_Fiona_L::Initialize()
{
    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_WALK_FIONA_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    return __super::Initialize();
}

void CState_Patrol_Walk_Fiona_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Walk_Fiona_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_iRand == 0)
            return AI_STATE_PATROL_DEFAULT_FIONA_L;
        else
            --m_iRand;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Walk_Fiona_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Walk_Fiona_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
