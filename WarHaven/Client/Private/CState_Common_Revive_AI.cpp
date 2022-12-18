#include "stdafx.h"
#include "CState_Common_Revive_AI.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CState_Common_Revive_AI::CState_Common_Revive_AI()
{
}

CState_Common_Revive_AI::~CState_Common_Revive_AI()
{
}

CState_Common_Revive_AI* CState_Common_Revive_AI::Create()
{
    CState_Common_Revive_AI* pInstance = new CState_Common_Revive_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Revive_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Revive_AI::Initialize()
{


    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 28;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_REVIVE_AI;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.5f;

    m_iStateChangeKeyFrame = 99;

    m_fAIMyLength = 2.f;

    return S_OK;
}

void CState_Common_Revive_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    CPlayer* pAbjPlayer = pOwner->Get_RevivalPlayer();

    _float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vPos = vMyPos;

    if (pAbjPlayer)
    {
        pAbjPlayer->Get_CurrentUnit()->Start_Reborn();
        vPos = pOwner->Get_RevivalPlayer()->Get_WorldPos();
    }
    else
    {
        m_eCurPhase = DANCE;
        m_iAnimIndex = 9;
    }


    _float4 vDir = vPos - vMyPos;
    pOwner->Get_Transform()->Set_LerpLook(vDir.Normalize(), 0.4f);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Revive_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // ���� ��Ÿ� �տ� ���� ������ �پ��ִٸ� ��Ȱ�� Ǯ�� ������ �ؾ߰���? ��� �ؾ��ұ��..

    switch (m_eCurPhase)
    {
    case Client::CState_Common_Revive_AI::DANCE:
        if (pAnimator->Is_CurAnimFinished())
        {
            STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
            return eDefaultState;
        }

        break;

    case Client::CState_Common_Revive_AI::BEGIN:

        if (pAnimator->Is_CurAnimFinished())
        {
            m_eCurPhase = LOOP;
            m_iAnimIndex = 29;
            __super::Enter(pOwner, pAnimator, m_eStateType);
        }

        break;
    case Client::CState_Common_Revive_AI::LOOP:
        if (KEY(F, NONE))
        {
            m_eCurPhase = PHASE_END;
            m_iAnimIndex = 30;
            __super::Enter(pOwner, pAnimator, m_eStateType);
        }

        break;
    case Client::CState_Common_Revive_AI::PHASE_END:
        if (pAnimator->Is_CurAnimFinished())
        {
            STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
            return eDefaultState;
        }
        break;
    default:
        break;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Revive_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CState_Common_Revive_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
