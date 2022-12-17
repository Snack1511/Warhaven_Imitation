#include "stdafx.h"
#include "CState_Common_Slide_End_AI.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"


CState_Common_Slide_End_AI::CState_Common_Slide_End_AI()
{
}

CState_Common_Slide_End_AI::~CState_Common_Slide_End_AI()
{
}

CState_Common_Slide_End_AI* CState_Common_Slide_End_AI::Create()
{
    CState_Common_Slide_End_AI* pInstance = new CState_Common_Slide_End_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Slide_End_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Slide_End_AI::Initialize()
{
    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 34;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_SLIDEEND_AI;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 99;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_Common_Slide_End_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Slide_End_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        STATE_TYPE eSprintEndState = pOwner->Get_SprintEndState();
        return eSprintEndState;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Slide_End_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
  
}

STATE_TYPE CState_Common_Slide_End_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

