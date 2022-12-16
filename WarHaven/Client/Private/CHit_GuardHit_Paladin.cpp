#include "stdafx.h"
#include "CHit_GuardHit_Paladin.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_GuardHit_Paladin::CHit_GuardHit_Paladin()
{
}

CHit_GuardHit_Paladin::~CHit_GuardHit_Paladin()
{
}

CHit_GuardHit_Paladin* CHit_GuardHit_Paladin::Create()
{
    CHit_GuardHit_Paladin* pInstance = new CHit_GuardHit_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_GuardHit_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_GuardHit_Paladin::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GUARDHIT_PALADIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 0;

  
    m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
    m_vecAdjState.push_back(STATE_RUN_PALADIN_R);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);


    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_GuardHit_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == STATE_RUSH_LOOP_PALADIN)
    {
        m_bMoveTrigger = true;
        return;
    }
    else
        m_bMoveTrigger = false;


    if (ePrevType == STATE_SHIELDWALL_LOOP_PALADIN ||
        ePrevType == STATE_SHIELDWALL_HIT_PALADIN)
    {
        m_bAttackTrigger = true;
        return;
    }
    else
        m_bAttackTrigger = false;



    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Guard_State(pOwner);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_GuardHit_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bMoveTrigger)
        return STATE_RUSH_LOOP_PALADIN;

    if (m_bAttackTrigger)
        return STATE_SHIELDWALL_HIT_PALADIN;


    return __super::Tick(pOwner, pAnimator);
}

void CHit_GuardHit_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_GuardHit_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN�� Attack ���� ���� ����
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
