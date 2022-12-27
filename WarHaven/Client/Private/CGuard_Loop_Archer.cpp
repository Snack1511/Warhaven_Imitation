#include "stdafx.h"
#include "CGuard_Loop_Archer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Loop_Archer::CGuard_Loop_Archer()
{
}

CGuard_Loop_Archer::~CGuard_Loop_Archer()
{
}

CGuard_Loop_Archer* CGuard_Loop_Archer::Create()
{
    CGuard_Loop_Archer* pInstance = new CGuard_Loop_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Loop_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Loop_Archer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	m_iStateChangeKeyFrame = 53;

	m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
	m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
	m_vecAdjState.push_back(STATE_RUN_ARCHER_R);

	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);
	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_ARCHER);
	m_vecAdjState.push_back(STATE_ATTACK_BEGIN_POISION_ARCHER);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);

	m_vecAdjState.push_back(STATE_ATTACK_SWING_ARCHER);

	Init_CommonState_Player();

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;


    return S_OK;
}

void CGuard_Loop_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Enable_GuardCollider(true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Loop_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_ARCHER_R;

	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	Follow_MouseLook(pOwner);


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Loop_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Loop_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (KEY(RBUTTON, TAP))
		return m_eStateType;
   
    return STATE_END;
}
