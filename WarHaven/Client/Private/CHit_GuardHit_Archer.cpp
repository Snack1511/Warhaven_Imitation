#include "stdafx.h"
#include "CHit_GuardHit_Archer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_GuardHit_Archer::CHit_GuardHit_Archer()
{
}

CHit_GuardHit_Archer::~CHit_GuardHit_Archer()
{
}

CHit_GuardHit_Archer* CHit_GuardHit_Archer::Create()
{
    CHit_GuardHit_Archer* pInstance = new CHit_GuardHit_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_GuardHit_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_GuardHit_Archer::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GUARDHIT_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 20;

  
    m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
    m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUN_ARCHER_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_ARCHER_R);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_R);
    //m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_ARCHER);
    //m_vecAdjState.push_back(STATE_GUARD_BEGIN_ARCHER);

    //m_vecAdjState.push_back(STATE_COUNTER_ARCHER);
    //m_vecAdjState.push_back(STATE_SPINATTACK_ARCHER);
    //m_vecAdjState.push_back(STATE_SHIELDATTACK_ARCHER);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_ARCHER_R);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_R);
    //m_vecAdjState.push_back(STATE_VERTICALATTACK_ARCHER_R);
    //m_vecAdjState.push_back(STATE_GUARD_BEGIN_ARCHER);


    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_GuardHit_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
    pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

    m_bMoveTrigger = false;

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Guard_State(pOwner);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_GuardHit_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CHit_GuardHit_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_GuardHit_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Attack ���� ���� ����
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
