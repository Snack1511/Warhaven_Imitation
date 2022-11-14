#include "stdafx.h"
#include "CBounce_Player_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_Player_L::CBounce_Player_L()
{
}

CBounce_Player_L::~CBounce_Player_L()
{
}

CBounce_Player_L* CBounce_Player_L::Create()
{
    CBounce_Player_L* pInstance = new CBounce_Player_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_Player_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_Player_L::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 16;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_UPPER_MIDDLE_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	m_vecAdjState.push_back(STATE_JUMP_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_UPPER_MIDDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

	m_vecAdjState.push_back(STATE_JUMPFALL_PLAYER_L);


    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);


    return S_OK;
}

void CBounce_Player_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CBounce_Player_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Player_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CBounce_Player_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  �����ϴ� ���� ƨ���
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        return m_eStateType;
    }
   
    return STATE_END;
}