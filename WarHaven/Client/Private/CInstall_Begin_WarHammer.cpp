#include "stdafx.h"
#include "CInstall_Begin_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit_WarHammer.h"

#include "CUser.h"


CInstall_Begin_WarHammer::CInstall_Begin_WarHammer()
{
}

CInstall_Begin_WarHammer::~CInstall_Begin_WarHammer()
{
}

CInstall_Begin_WarHammer* CInstall_Begin_WarHammer::Create()
{
    CInstall_Begin_WarHammer* pInstance = new CInstall_Begin_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CInstall_Begin_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CInstall_Begin_WarHammer::Initialize()
{
	m_eAnimType = ANIM_ETC;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_INSTALL_BEIGN_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(STATE_INSTALL_LOOP_WARHAMMER);
	m_vecAdjState.push_back(STATE_INSTALL_END_WARHAMMER);


    return S_OK;
}

void CInstall_Begin_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	CUnit_WarHammer* PWarHammer = static_cast<CUnit_WarHammer*>(pOwner);

	__super::Enable_ModelParts(PWarHammer, 4, false);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CInstall_Begin_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.
	Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CInstall_Begin_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Enable_ModelParts(pOwner, 4, true);
}

STATE_TYPE CInstall_Begin_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Sprint�� ���� ����
	1. ������Ʈ ���� �غ� ������
    */

	CUnit_WarHammer* PWarHammer = static_cast<CUnit_WarHammer*>(pOwner);

	if (KEY(R, TAP) && PWarHammer->Get_Size() > 0)
		return m_eStateType;
   
    return STATE_END;
}
