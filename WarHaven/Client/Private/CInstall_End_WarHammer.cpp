#include "stdafx.h"
#include "CInstall_End_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CUnit_WarHammer.h"

CInstall_End_WarHammer::CInstall_End_WarHammer()
{
}

CInstall_End_WarHammer::~CInstall_End_WarHammer()
{
}

CInstall_End_WarHammer* CInstall_End_WarHammer::Create()
{
    CInstall_End_WarHammer* pInstance = new CInstall_End_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CInstall_End_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CInstall_End_WarHammer::Initialize()
{
	m_eAnimType = ANIM_ETC;          // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 4;                // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_INSTALL_END_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	// ���� ���� �ð�
	m_fInterPolationTime = 0.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);

    return S_OK;
}

void CInstall_End_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CInstall_End_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���� ���¿����� ������ �� �ֵ��� �����Ѵ�.


    return __super::Tick(pOwner, pAnimator);

}

void CInstall_End_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CInstall_End_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(RBUTTON, TAP))
	{
		return m_eStateType;
	}

	if (pAnimator->Is_CurAnimFinished())
	{
		ENABLE_GAMEOBJECT(dynamic_cast<CUnit_WarHammer*>(pOwner)->Get_Barricade());
		//dynamic_cast<CUnit_WarHammer*>(pOwner)->Creaete_Barricade();
		return m_eStateType;
	}
	
    return STATE_END;
}
