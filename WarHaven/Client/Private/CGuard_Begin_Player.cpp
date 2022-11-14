#include "stdafx.h"
#include "CGuard_Begin_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Begin_Player::CGuard_Begin_Player()
{
}

CGuard_Begin_Player::~CGuard_Begin_Player()
{
}

CGuard_Begin_Player* CGuard_Begin_Player::Create()
{
    CGuard_Begin_Player* pInstance = new CGuard_Begin_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Begin_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Begin_Player::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_BEGIN_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(STATE_GUARD_LOOP_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_END_PLAYER);


    return S_OK;
}

void CGuard_Begin_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Begin_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Begin_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CGuard_Begin_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */



	if (KEY(RBUTTON, TAP))
		return m_eStateType;
   
    return STATE_END;
}
