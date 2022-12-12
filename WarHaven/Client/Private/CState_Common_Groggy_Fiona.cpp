#include "stdafx.h"
#include "CState_Common_Groggy_Fiona.h"

#include "GameInstance.h"
#include "Physics.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

#include "CUtility_Transform.h"

CState_Common_Groggy_Fiona::CState_Common_Groggy_Fiona()
{
}

CState_Common_Groggy_Fiona::~CState_Common_Groggy_Fiona()
{
}

CState_Common_Groggy_Fiona* CState_Common_Groggy_Fiona::Create()
{
    CState_Common_Groggy_Fiona* pInstance = new CState_Common_Groggy_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Groggy_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_Groggy_Fiona::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 2;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_GROGGYHIT_FIONA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.5f;


    return S_OK;
}

void CState_Common_Groggy_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (!pData)
    {
        __super::Enter(pOwner, pAnimator, ePrevType, pData);
        return;
    }

    /* �� �������� hit info�� �޾Ҵ�. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Groggy_State(pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Groggy_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if(pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEAFULT_FIONA_R;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Groggy_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Groggy_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
