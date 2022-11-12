#include "stdafx.h"
#include "CGuard_Cancel_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Cancel_Player::CGuard_Cancel_Player()
{
}

CGuard_Cancel_Player::~CGuard_Cancel_Player()
{
}

CGuard_Cancel_Player* CGuard_Cancel_Player::Create()
{
    CGuard_Cancel_Player* pInstance = new CGuard_Cancel_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Cancel_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Cancel_Player::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_CANCEL_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

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

void CGuard_Cancel_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CGuard_Cancel_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Cancel_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CGuard_Cancel_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. Hit ����.
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}
