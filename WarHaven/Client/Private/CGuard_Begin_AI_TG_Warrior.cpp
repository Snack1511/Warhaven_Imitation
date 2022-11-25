#include "stdafx.h"
#include "CGuard_Begin_AI_TG_Warrior.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_AI_TG_Warrior::CGuard_Begin_AI_TG_Warrior()
{
}

CGuard_Begin_AI_TG_Warrior::~CGuard_Begin_AI_TG_Warrior()
{
}

CGuard_Begin_AI_TG_Warrior* CGuard_Begin_AI_TG_Warrior::Create()
{
    CGuard_Begin_AI_TG_Warrior* pInstance = new CGuard_Begin_AI_TG_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_AI_TG_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_AI_TG_Warrior::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_GUARD_BEGIN_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(AI_STATE_GUARD_LOOP_WARRIOR);
	m_vecAdjState.push_back(AI_STATE_GUARD_END_WARRIOR);


    return S_OK;
}

void CGuard_Begin_AI_TG_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_AI_TG_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	//Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_AI_TG_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Begin_AI_TG_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */



	//if (KEY(RBUTTON, HOLD))
	//	return m_eStateType;
   
    return STATE_END;
}
