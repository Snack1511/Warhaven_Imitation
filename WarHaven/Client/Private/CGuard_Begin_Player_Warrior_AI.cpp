#include "stdafx.h"
#include "CGuard_Begin_Player_Warrior_AI.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_Player_Warrior_AI::CGuard_Begin_Player_Warrior_AI()
{
}

CGuard_Begin_Player_Warrior_AI::~CGuard_Begin_Player_Warrior_AI()
{
}

CGuard_Begin_Player_Warrior_AI* CGuard_Begin_Player_Warrior_AI::Create()
{
    CGuard_Begin_Player_Warrior_AI* pInstance = new CGuard_Begin_Player_Warrior_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_Player_Warrior_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_Player_Warrior_AI::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(STATE_GUARD_LOOP_WARRIOR_AI_ENEMY);
	// m_vecAdjState.push_back(STATE_GUARD_END_PLAYER);


    return S_OK;
}

void CGuard_Begin_Player_Warrior_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_Player_Warrior_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.

    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_Player_Warrior_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_Begin_Player_Warrior_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   

    return STATE_END;
}
