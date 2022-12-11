#include "stdafx.h"
#include "CState_Combat_Oxen_Cancel_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CState_Combat_Oxen_Cancel_Warrior::CState_Combat_Oxen_Cancel_Warrior()
{
}

CState_Combat_Oxen_Cancel_Warrior::~CState_Combat_Oxen_Cancel_Warrior()
{
}

CState_Combat_Oxen_Cancel_Warrior* CState_Combat_Oxen_Cancel_Warrior::Create()
{
    CState_Combat_Oxen_Cancel_Warrior* pInstance = new CState_Combat_Oxen_Cancel_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Oxen_Cancel_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Oxen_Cancel_Warrior::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 22;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_END;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 20;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);

    return S_OK;
}

void CState_Combat_Oxen_Cancel_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->TurnOn_TrailEffect(true);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Oxen_Cancel_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // Ȳ�Һ��� ĵ��(���� 3��)
    if(pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PLAYER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Oxen_Cancel_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CState_Combat_Oxen_Cancel_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}



