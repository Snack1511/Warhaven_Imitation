#include "stdafx.h"
#include "CGuard_Loop_Valkyrie.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Loop_Valkyrie::CGuard_Loop_Valkyrie()
{
}

CGuard_Loop_Valkyrie::~CGuard_Loop_Valkyrie()
{
}

CGuard_Loop_Valkyrie* CGuard_Loop_Valkyrie::Create()
{
    CGuard_Loop_Valkyrie* pInstance = new CGuard_Loop_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Loop_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Loop_Valkyrie::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 45;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_LOOP_VALKYRIE;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;

	//m_vecAdjState.push_back(STATE_GUARD_CANCEL_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_END_VALKYRIE);



    return S_OK;
}

void CGuard_Loop_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Enable_GuardCollider(true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Loop_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	Follow_MouseLook(pOwner);


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Loop_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Loop_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (pAnimator->Is_CurAnimFinished())
	{
		if (KEY(RBUTTON, HOLD))
			return m_eStateType;
	}

   
    return STATE_END;
}
