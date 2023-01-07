#include "stdafx.h"
#include "CState_PathNavigation_Fall_Archer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"

CState_PathNavigation_Fall_Archer_R::CState_PathNavigation_Fall_Archer_R()
{
}

CState_PathNavigation_Fall_Archer_R::~CState_PathNavigation_Fall_Archer_R()
{
}


HRESULT CState_PathNavigation_Fall_Archer_R::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_FALL_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_eAILandStateType = AI_STATE_COMMON_LAND_ARCHER_R;

    return S_OK;
}

CState_PathNavigation_Fall_Archer_R* CState_PathNavigation_Fall_Archer_R::Create()
{
    CState_PathNavigation_Fall_Archer_R* pInstance = new CState_PathNavigation_Fall_Archer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Fall_Archer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CState_PathNavigation_Fall_Archer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    //if (ePrevType == STATE_JUMP_PLAYER_R)
    //    m_fInterPolationTime = 0.05f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Fall_Archer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Fall_Archer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Fall_Archer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
