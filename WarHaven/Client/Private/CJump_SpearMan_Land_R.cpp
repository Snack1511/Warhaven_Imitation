#include "stdafx.h"
#include "CJump_SpearMan_Land_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_SpearMan_Land_R::CJump_SpearMan_Land_R()
{
}

CJump_SpearMan_Land_R::~CJump_SpearMan_Land_R()
{
}

CJump_SpearMan_Land_R* CJump_SpearMan_Land_R::Create()
{
    CJump_SpearMan_Land_R* pInstance = new CJump_SpearMan_Land_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_SpearMan_Land_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_SpearMan_Land_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 17;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_SPEARMAN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    
//	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);

	m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);

	m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN_R);

	//m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	//m_vecAdjState.push_back(STATE_GUARD_BEGIN_SPEARMAN);


	//m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	//m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);

 //   m_vecAdjState.push_back(STATE_SPRINT_BEGIN_SPEARMAN);


    return S_OK;
}

void CJump_SpearMan_Land_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_SpearMan_Land_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CJump_SpearMan_Land_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_SpearMan_Land_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
