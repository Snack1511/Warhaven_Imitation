#include "stdafx.h"
#include "CGuard_Loop_Warrior_AI.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Loop_Warrior_AI::CGuard_Loop_Warrior_AI()
{
}

CGuard_Loop_Warrior_AI::~CGuard_Loop_Warrior_AI()
{
}

CGuard_Loop_Warrior_AI* CGuard_Loop_Warrior_AI::Create()
{
    CGuard_Loop_Warrior_AI* pInstance = new CGuard_Loop_Warrior_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Loop_Warrior_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Loop_Warrior_AI::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 5;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_LOOP_WARRIOR_AI_ENEMY;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;

	//m_vecAdjState.push_back(STATE_GUARD_CANCEL_PLAYER);
	//m_vecAdjState.push_back(STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY);



    return S_OK;
}

void CGuard_Loop_Warrior_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{


	pOwner->Enable_GuardCollider(true);

	pOwner->CallBack_CollisionEnter += bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Loop_Warrior_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Set_LookToTarget();


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Loop_Warrior_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->CallBack_CollisionEnter -= bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

    /* �Ұž��� */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CGuard_Loop_Warrior_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

   
    return STATE_END;
}
