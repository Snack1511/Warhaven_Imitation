#include "stdafx.h"
#include "CSwitchLtoR.h"

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

CSwitchLtoR::CSwitchLtoR()
{
}

CSwitchLtoR::~CSwitchLtoR()
{
}

CSwitchLtoR* CSwitchLtoR::Create()
{
    CSwitchLtoR* pInstance = new CSwitchLtoR();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchLtoR");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchLtoR::Initialize()
{
    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 27;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_L_TO_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 10;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.2f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 4.2f;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_ATTACK_UPPER_MIDDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);


    return S_OK;
}

void CSwitchLtoR::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    




    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CSwitchLtoR::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_UPPER_MIDDLE_PLAYER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchLtoR::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSwitchLtoR::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::R)
        return m_eStateType;

    return STATE_END;
}
