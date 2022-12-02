#include "stdafx.h"
#include "CGuard_Begin_Valkyrie.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_Valkyrie::CGuard_Begin_Valkyrie()
{
}

CGuard_Begin_Valkyrie::~CGuard_Begin_Valkyrie()
{
}

CGuard_Begin_Valkyrie* CGuard_Begin_Valkyrie::Create()
{
    CGuard_Begin_Valkyrie* pInstance = new CGuard_Begin_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_Valkyrie::Initialize()
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

void CGuard_Begin_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Begin_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */



	if (KEY(RBUTTON, HOLD))
		return m_eStateType;
   
    return STATE_END;
}
