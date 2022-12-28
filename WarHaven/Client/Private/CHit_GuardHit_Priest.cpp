#include "stdafx.h"
#include "CHit_GuardHit_Priest.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_GuardHit_Priest::CHit_GuardHit_Priest()
{
}

CHit_GuardHit_Priest::~CHit_GuardHit_Priest()
{
}

CHit_GuardHit_Priest* CHit_GuardHit_Priest::Create()
{
    CHit_GuardHit_Priest* pInstance = new CHit_GuardHit_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_GuardHit_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_GuardHit_Priest::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GUARDHIT_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 30;
    
    m_vecAdjState.push_back(STATE_IDLE_PRIEST);
    m_vecAdjState.push_back(STATE_WALK_PRIEST);
    m_vecAdjState.push_back(STATE_RUN_PRIEST);
    m_vecAdjState.push_back(STATE_ATTACK_STING_PRIEST);



    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_GuardHit_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_bMoveTrigger = false;

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Guard_State(pOwner);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_GuardHit_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //if (KEY(SPACE, TAP))
    //    m_bMoveTrigger = true;

    //if (m_bMoveTrigger || KEY(S, TAP))
    //    return STATE_GUARDDASH_PRIEST;
    

    return __super::Tick(pOwner, pAnimator);
}

void CHit_GuardHit_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_GuardHit_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PRIEST�� Attack ���� ���� ����
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
