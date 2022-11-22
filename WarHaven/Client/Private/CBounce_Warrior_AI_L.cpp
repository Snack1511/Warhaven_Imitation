#include "stdafx.h"
#include "CBounce_Warrior_AI_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_Warrior_AI_L::CBounce_Warrior_AI_L()
{
}

CBounce_Warrior_AI_L::~CBounce_Warrior_AI_L()
{
}

CBounce_Warrior_AI_L* CBounce_Warrior_AI_L::Create()
{
    CBounce_Warrior_AI_L* pInstance = new CBounce_Warrior_AI_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_Warrior_AI_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_Warrior_AI_L::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 16;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_WARRIOR_L_AI_ENEMY;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	//m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);



	////m_vecAdjState.push_back(STATE_JUMPFALL_PLAYER_L);

 //   m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
 //   m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
 //   m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_L);


    return S_OK;
}

void CBounce_Warrior_AI_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_Warrior_AI_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY;

    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Warrior_AI_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CBounce_Warrior_AI_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  �����ϴ� ���� ƨ���
    */

	return __super::Check_Condition(pOwner, pAnimator);
 
}