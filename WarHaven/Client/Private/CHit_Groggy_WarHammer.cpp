#include "stdafx.h"
#include "CHit_Groggy_WarHammer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CHit_Groggy_WarHammer::CHit_Groggy_WarHammer()
{
}

CHit_Groggy_WarHammer::~CHit_Groggy_WarHammer()
{
}

CHit_Groggy_WarHammer* CHit_Groggy_WarHammer::Create()
{
    CHit_Groggy_WarHammer* pInstance = new CHit_Groggy_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Groggy_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_Groggy_WarHammer::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GROGGYHIT_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 105;
    
    m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);

    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_Groggy_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Groggy_State();

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_Groggy_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bMoveTrigger)
		DoMove(Get_Direction(), pOwner);

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Groggy_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_Groggy_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
