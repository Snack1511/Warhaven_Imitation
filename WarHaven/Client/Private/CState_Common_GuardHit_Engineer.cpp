#include "stdafx.h"
#include "CState_Common_GuardHit_Engineer.h"

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

CState_Common_GuardHit_Engineer::CState_Common_GuardHit_Engineer()
{
}

CState_Common_GuardHit_Engineer::~CState_Common_GuardHit_Engineer()
{
}

CState_Common_GuardHit_Engineer* CState_Common_GuardHit_Engineer::Create()
{
    CState_Common_GuardHit_Engineer* pInstance = new CState_Common_GuardHit_Engineer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_GuardHit_Engineer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_GuardHit_Engineer::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iGuardIndex[0];                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_GUARDHIT_ENGINEER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    

    return S_OK;
}

void CState_Common_GuardHit_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* �� �������� hit info�� �޾Ҵ�. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Guard_State(pOwner);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_GuardHit_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Get_CurAnimFrame() > 10)
        return AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R;


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_GuardHit_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_GuardHit_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
