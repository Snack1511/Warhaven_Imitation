#include "stdafx.h"
#include "CBounce_Paladin_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_Paladin_L::CBounce_Paladin_L()
{
}

CBounce_Paladin_L::~CBounce_Paladin_L()
{
}

CBounce_Paladin_L* CBounce_Paladin_L::Create()
{
    CBounce_Paladin_L* pInstance = new CBounce_Paladin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_Paladin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_Paladin_L::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_VALKYRIE_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_WALK_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_L);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_L);

    return S_OK;
}

void CBounce_Paladin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_Paladin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Paladin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CBounce_Paladin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  �����ϴ� ���� ƨ���
    */

	return __super::Check_Condition(pOwner, pAnimator);
 
}