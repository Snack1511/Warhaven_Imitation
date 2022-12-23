#include "stdafx.h"
#include "CGuard_Loop_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Loop_Qanda::CGuard_Loop_Qanda()
{
}

CGuard_Loop_Qanda::~CGuard_Loop_Qanda()
{
}

CGuard_Loop_Qanda* CGuard_Loop_Qanda::Create()
{
    CGuard_Loop_Qanda* pInstance = new CGuard_Loop_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Loop_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Loop_Qanda::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 38;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	m_iStateChangeKeyFrame = 53;

	m_vecAdjState.push_back(STATE_IDLE_QANDA);
	m_vecAdjState.push_back(STATE_WALK_QANDA);
	m_vecAdjState.push_back(STATE_RUN_QANDA);

	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_QANDA);
	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_QANDA);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_QANDA);

	m_vecAdjState.push_back(STATE_SHADOWSTEP_QANDA);

	Init_CommonState_Player();

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;


    return S_OK;
}

void CGuard_Loop_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Loop_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_QANDA;

	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	Follow_MouseLook(pOwner);


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Loop_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_Loop_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (KEY(RBUTTON, TAP))
		return m_eStateType;
   
    return STATE_END;
}
