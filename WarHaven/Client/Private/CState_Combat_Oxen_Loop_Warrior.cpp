#include "stdafx.h"
#include "CState_Combat_Oxen_Loop_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CState_Combat_Oxen_Loop_Warrior::CState_Combat_Oxen_Loop_Warrior()
{
}

CState_Combat_Oxen_Loop_Warrior::~CState_Combat_Oxen_Loop_Warrior()
{
}

CState_Combat_Oxen_Loop_Warrior* CState_Combat_Oxen_Loop_Warrior::Create()
{
    CState_Combat_Oxen_Loop_Warrior* pInstance = new CState_Combat_Oxen_Loop_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Oxen_Loop_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Oxen_Loop_Warrior::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 25;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_LOOP;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 3.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 2;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_LOOPATTACK);

    m_eBounceState = STATE_BOUNCE_PLAYER_R;

    m_iEndFinishedFrame = 3;

    Add_KeyFrame(47, 1);

    return S_OK;
}

void CState_Combat_Oxen_Loop_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->TurnOn_TrailEffect(true);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Oxen_Loop_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
    {
        ++m_iCurFinishedFrame;

        if (m_iCurFinishedFrame == m_iEndFinishedFrame)
        {
            m_iCurFinishedFrame = 0;
            return STATE_WARRIOR_OXEN_END;
        }
    }

    // Ȳ�� ����� ������ ���� ���� �͵��� �־������.
    // �⺻������ ������ ������ ������ �ϱ� ������ Base_L �� �ٲ��ָ��.


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Oxen_Loop_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CState_Combat_Oxen_Loop_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Oxen ���� ���� ����
    1.  Q �� �̿��� �����Ѵ�.
        */

    return STATE_END;
}

void CState_Combat_Oxen_Loop_Warrior::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {

    case 1:
        Play_Sound(L"Effect_Oxen_Loop_Attack_Warrior", CHANNEL_EFFECTS, 1.f);
        m_bAttackTrigger = true;
        pOwner->Enable_FlyAttackCollider(true);
        Play_Voice(pOwner, L"Voice_Attack", 1.f);
        break;
    default:
        break;
    }


}

