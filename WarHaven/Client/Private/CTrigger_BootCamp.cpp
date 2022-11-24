#include "stdafx.h"
#include "CTrigger_BootCamp.h"

#include "CGameSystem.h"

#include "UsefulHeaders.h"

CTrigger_BootCamp::CTrigger_BootCamp()
{
}

CTrigger_BootCamp::~CTrigger_BootCamp()
{
}

void CTrigger_BootCamp::Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	//1. �浹�� ����� �÷��̾����� üũ
	if (eOtherColType == COL_PLAYERTEAM)
	{
		if (static_cast<CUnit*>(pOtherObj)->Is_MainPlayer())
		{
			//�� �ε����� UI Ű��
			m_iUIIndex;
		}
	}


}

void CTrigger_BootCamp::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger_BootCamp::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	//1. �浹�� ����� �÷��̾����� üũ
	if (eOtherColType == COL_PLAYERTEAM)
	{
		if (static_cast<CUnit*>(pOtherObj)->Is_MainPlayer())
		{
			//�� �ε����� UI ����
			m_iUIIndex;
		}
	}
}

CTrigger_BootCamp* CTrigger_BootCamp::Create(string strPositionKey, _uint iUIIndex, _float fRadius)
{
	CTrigger_BootCamp* pInstance = new CTrigger_BootCamp;

	pInstance->m_vPosition = CGameSystem::Get_Instance()->Find_Position(strPositionKey);
	pInstance->m_pTransform->Set_World(WORLD_POS, pInstance->m_vPosition);
	pInstance->m_pTransform->Make_WorldMatrix();
	pInstance->m_iUIIndex = iUIIndex;
	pInstance->m_fRadius = fRadius;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CTrigger_BootCamp");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CTrigger_BootCamp::Initialize_Prototype()
{
	m_eColGroup = COL_TRIGGER;

	return __super::Initialize_Prototype();
}
