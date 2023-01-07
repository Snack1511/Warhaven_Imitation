#include "stdafx.h"
#include "CState_PathNavigation_Jump_Engineer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Jump_Engineer_L::CState_PathNavigation_Jump_Engineer_L()
{
}

CState_PathNavigation_Jump_Engineer_L::~CState_PathNavigation_Jump_Engineer_L()
{
}

CState_PathNavigation_Jump_Engineer_L* CState_PathNavigation_Jump_Engineer_L::Create()
{
    CState_PathNavigation_Jump_Engineer_L* pInstance = new CState_PathNavigation_Jump_Engineer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Jump_Engineer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Jump_Engineer_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 5;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_JUMP_ENGINEER_L; // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

 
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 8;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 9;


    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 8;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 8;

	iPlaceJumpAnimIndex = 5;

    m_iAINextState = AI_STATE_PATHNAVIGATION_FALL_ENGINEER_L;


    return S_OK;
}

void CState_PathNavigation_Jump_Engineer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Engineer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CState_PathNavigation_Jump_Engineer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CState_PathNavigation_Jump_Engineer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
