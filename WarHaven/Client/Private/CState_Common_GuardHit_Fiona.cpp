#include "stdafx.h"
#include "CState_Common_GuardHit_Fiona.h"

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

CState_Common_GuardHit_Fiona::CState_Common_GuardHit_Fiona()
{
}

CState_Common_GuardHit_Fiona::~CState_Common_GuardHit_Fiona()
{
}

CState_Common_GuardHit_Fiona* CState_Common_GuardHit_Fiona::Create()
{
    CState_Common_GuardHit_Fiona* pInstance = new CState_Common_GuardHit_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_GuardHit_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_GuardHit_Fiona::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iGuardIndex[0];                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_GUARDHIT_FIONA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_Common_GuardHit_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* �� �������� hit info�� �޾Ҵ�. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Guard_State(pOwner);

    if (ePrevType == AI_STATE_COMBAT_COUNTER_FIONA ||
        ePrevType == m_eStateType)
        m_bAttackTrigger = true;

    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_GuardHit_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
        return eDefaultState;
    }

    if (m_bAttackTrigger)
        return AI_STATE_COMBAT_SPINATTACK_FIONA;

    if (pAnimator->Get_CurAnimFrame() > 10)
        return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L;


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_GuardHit_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_GuardHit_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
