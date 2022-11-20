#include "stdafx.h"
#include "CSwitchLtoR_WarHammer.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CSwitchLtoR_WarHammer::CSwitchLtoR_WarHammer()
{
}

CSwitchLtoR_WarHammer::~CSwitchLtoR_WarHammer()
{
}

CSwitchLtoR_WarHammer* CSwitchLtoR_WarHammer::Create()
{
    CSwitchLtoR_WarHammer* pInstance = new CSwitchLtoR_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchLtoR_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchLtoR_WarHammer::Initialize()
{
    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 28;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_L_TO_R_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);



    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);


    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


    return S_OK;
}

void CSwitchLtoR_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    




    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSwitchLtoR_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::V)
        return STATE_CHARGE_FRONT_WARHAMMER_R;

    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return STATE_VERTICALATTACK_WARHAMMER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchLtoR_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSwitchLtoR_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::TAB)
        return m_eStateType;

    return STATE_END;
}
