#include "stdafx.h"
#include "CGuard_Cancel_Archer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Cancel_Archer::CGuard_Cancel_Archer()
{
}

CGuard_Cancel_Archer::~CGuard_Cancel_Archer()
{
}

CGuard_Cancel_Archer* CGuard_Cancel_Archer::Create()
{
    CGuard_Cancel_Archer* pInstance = new CGuard_Cancel_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Cancel_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Cancel_Archer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 2;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_CANCEL_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 75;

    m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
    m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUN_ARCHER_R);

    m_vecAdjState.push_back(STATE_GUARD_ARCHER);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_POISION_ARCHER);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);

    return S_OK;
}

void CGuard_Cancel_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Cancel_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Cancel_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CGuard_Cancel_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. Hit ����.
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}
