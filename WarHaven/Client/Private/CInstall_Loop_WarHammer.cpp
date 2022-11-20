#include "stdafx.h"
#include "CInstall_Loop_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit_WarHammer.h"

#include "CUser.h"

CInstall_Loop_WarHammer::CInstall_Loop_WarHammer()
{
}

CInstall_Loop_WarHammer::~CInstall_Loop_WarHammer()
{
}

CInstall_Loop_WarHammer* CInstall_Loop_WarHammer::Create()
{
    CInstall_Loop_WarHammer* pInstance = new CInstall_Loop_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CInstall_Loop_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CInstall_Loop_WarHammer::Initialize()
{
	m_eAnimType = ANIM_ETC;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 5;                // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_INSTALL_LOOP_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.f;

	m_vecAdjState.push_back(STATE_INSTALL_END_WARHAMMER);

    return S_OK;
}

void CInstall_Loop_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	CUnit_WarHammer* PWarHammer = static_cast<CUnit_WarHammer*>(pOwner);

	pOwner->On_Use(CUnit::SKILL3);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CInstall_Loop_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    return __super::Tick(pOwner, pAnimator);
}

void CInstall_Loop_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CInstall_Loop_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

   
    return STATE_END;
}
