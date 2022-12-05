#include "stdafx.h"
#include "CGuard_Begin_Paladin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_Paladin::CGuard_Begin_Paladin()
{
}

CGuard_Begin_Paladin::~CGuard_Begin_Paladin()
{
}

CGuard_Begin_Paladin* CGuard_Begin_Paladin::Create()
{
    CGuard_Begin_Paladin* pInstance = new CGuard_Begin_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_Paladin::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 2;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_BEGIN_VALKYRIE;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(STATE_GUARD_LOOP_VALKYRIE);
	m_vecAdjState.push_back(STATE_GUARD_END_VALKYRIE);


    return S_OK;
}

void CGuard_Begin_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Begin_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */



	if (KEY(RBUTTON, HOLD))
		return m_eStateType;
   
    return STATE_END;
}
