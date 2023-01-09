#include "stdafx.h"
#include "CHit_Lancer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"
#include "CProjectile.h"

CHit_Lancer::CHit_Lancer()
{
}

CHit_Lancer::~CHit_Lancer()
{
}

CHit_Lancer* CHit_Lancer::Create()
{
    CHit_Lancer* pInstance = new CHit_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_Lancer::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 12;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_HIT_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.��
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 50;
    
    m_vecAdjState.push_back(STATE_RUN_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_STING_LANCER);

    m_fMyAccel = 1.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.1f;

    return __super::Initialize();
}

void CHit_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Hit_State(pOwner);
        

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_LANCER;

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  LBuutton �� �̿��� �����Ѵ�.
    */
    //if (KEY(CTRL, NONE))
    //{
    //    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    //    {
    //        return m_eStateType;
    //    }
    //}
   
    return STATE_END;
}
