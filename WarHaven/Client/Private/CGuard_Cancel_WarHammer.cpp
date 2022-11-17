#include "stdafx.h"
#include "CGuard_Cancel_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Cancel_WarHammer::CGuard_Cancel_WarHammer()
{
}

CGuard_Cancel_WarHammer::~CGuard_Cancel_WarHammer()
{
}

CGuard_Cancel_WarHammer* CGuard_Cancel_WarHammer::Create()
{
    CGuard_Cancel_WarHammer* pInstance = new CGuard_Cancel_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Cancel_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Cancel_WarHammer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_CANCEL_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 100;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_R);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_R);


    return S_OK;
}

void CGuard_Cancel_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Cancel_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Cancel_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CGuard_Cancel_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. Hit ����.
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}
