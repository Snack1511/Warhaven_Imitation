#include "stdafx.h"
#include "CGuard_Loop_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Loop_WarHammer::CGuard_Loop_WarHammer()
{
}

CGuard_Loop_WarHammer::~CGuard_Loop_WarHammer()
{
}

CGuard_Loop_WarHammer* CGuard_Loop_WarHammer::Create()
{
    CGuard_Loop_WarHammer* pInstance = new CGuard_Loop_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Loop_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Loop_WarHammer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_LOOP_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;

	m_vecAdjState.push_back(STATE_GUARD_END_WARHAMMER);

    return S_OK;
}

void CGuard_Loop_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Loop_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	Follow_MouseLook(pOwner);


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Loop_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CGuard_Loop_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (pAnimator->Is_CurAnimFinished())
	{
		if (KEY(RBUTTON, HOLD))
			return m_eStateType;
	}

   
    return STATE_END;
}
