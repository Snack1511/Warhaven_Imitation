#include "stdafx.h"
#include "CHit_Groggy_Qanda.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_Groggy_Qanda::CHit_Groggy_Qanda()
{
}

CHit_Groggy_Qanda::~CHit_Groggy_Qanda()
{
}

CHit_Groggy_Qanda* CHit_Groggy_Qanda::Create()
{
    CHit_Groggy_Qanda* pInstance = new CHit_Groggy_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Groggy_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_Groggy_Qanda::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GROGGYHIT_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 105;
    
    m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
 //   m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
 //   m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);
 //   m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);
 //   m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);

    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_Groggy_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);

    pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
    pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Groggy_State(pOwner);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_Groggy_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Groggy_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_Groggy_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Attack ���� ���� ����
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
