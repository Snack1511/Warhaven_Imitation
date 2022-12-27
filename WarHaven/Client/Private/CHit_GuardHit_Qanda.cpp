#include "stdafx.h"
#include "CHit_GuardHit_Qanda.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_GuardHit_Qanda::CHit_GuardHit_Qanda()
{
}

CHit_GuardHit_Qanda::~CHit_GuardHit_Qanda()
{
}

CHit_GuardHit_Qanda* CHit_GuardHit_Qanda::Create()
{
    CHit_GuardHit_Qanda* pInstance = new CHit_GuardHit_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_GuardHit_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_GuardHit_Qanda::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GUARDHIT_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 20;

  
    m_vecAdjState.push_back(STATE_IDLE_QANDA);
    m_vecAdjState.push_back(STATE_WALK_QANDA);
    m_vecAdjState.push_back(STATE_RUN_QANDA);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_QANDA);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_QANDA);
    //m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_QANDA);
    //m_vecAdjState.push_back(STATE_GUARD_BEGIN_QANDA);

    //m_vecAdjState.push_back(STATE_COUNTER_QANDA);
    //m_vecAdjState.push_back(STATE_SPINATTACK_QANDA);
    //m_vecAdjState.push_back(STATE_SHIELDATTACK_QANDA);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_QANDA);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_QANDA);
    //m_vecAdjState.push_back(STATE_VERTICALATTACK_QANDA);
    //m_vecAdjState.push_back(STATE_GUARD_BEGIN_QANDA);


    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_GuardHit_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);

    pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
    pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

    m_bMoveTrigger = false;

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Guard_State(pOwner);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_GuardHit_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CHit_GuardHit_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_GuardHit_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* QANDA�� Attack ���� ���� ����
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
