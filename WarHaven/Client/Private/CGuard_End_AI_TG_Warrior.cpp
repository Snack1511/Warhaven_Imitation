#include "stdafx.h"
#include "CGuard_End_AI_TG_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_End_AI_TG_Warrior::CGuard_End_AI_TG_Warrior()
{
}

CGuard_End_AI_TG_Warrior::~CGuard_End_AI_TG_Warrior()
{
}

CGuard_End_AI_TG_Warrior* CGuard_End_AI_TG_Warrior::Create()
{
    CGuard_End_AI_TG_Warrior* pInstance = new CGuard_End_AI_TG_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_End_AI_TG_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_End_AI_TG_Warrior::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_GUARD_END_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

    m_vecAdjState.push_back(AI_STATE_IDLE_WARRIOR_L);
    m_vecAdjState.push_back(AI_STATE_RUN_WARRIOR_L);
    m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_L);

    return S_OK;
}

void CGuard_End_AI_TG_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_End_AI_TG_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_End_AI_TG_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_End_AI_TG_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	//if (KEY(RBUTTON, NONE))
	//	return m_eStateType;
   
    return STATE_END;
}
