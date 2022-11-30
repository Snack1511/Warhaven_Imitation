#include "stdafx.h"
#include "CTrigger_Paden.h"

#include "CGameSystem.h"

#include "UsefulHeaders.h"

#include "CPlayer.h"

CTrigger_Paden::CTrigger_Paden()
{
}

CTrigger_Paden::~CTrigger_Paden()
{
}

void CTrigger_Paden::Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	//1. 충돌한 대상이 플레이어인지 체크
	if (eOtherColType == COL_PLAYERTEAM)
	{
		if (static_cast<CUnit*>(pOtherObj)->Is_MainPlayer())
		{
			
		}
	}


}

void CTrigger_Paden::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger_Paden::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	//1. 충돌한 대상이 플레이어인지 체크
	if (eOtherColType == COL_PLAYERTEAM)
	{
		if (static_cast<CUnit*>(pOtherObj)->Is_MainPlayer())
		{

		}
	}
}

CTrigger_Paden* CTrigger_Paden::Create(string strPositionKey, _uint iUIIndex, _float fRadius)
{
	CTrigger_Paden* pInstance = new CTrigger_Paden;

	pInstance->m_vPosition = CGameSystem::Get_Instance()->Find_Position(strPositionKey);
	pInstance->m_pTransform->Set_World(WORLD_POS, pInstance->m_vPosition);
	pInstance->m_pTransform->Make_WorldMatrix();
	pInstance->m_iUIIndex = iUIIndex;
	pInstance->m_fRadius = fRadius;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CTrigger_Paden");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

_float4 CTrigger_Paden::Get_RespawnPosition()
{
	_float4 vPos = m_vRespawnPositions.front();
	m_vRespawnPositions.pop_front();
	m_vRespawnPositions.push_back(vPos);
	return vPos;
}

HRESULT CTrigger_Paden::Initialize_Prototype()
{
	m_eColGroup = COL_TRIGGER;

	return __super::Initialize_Prototype();
}

HRESULT CTrigger_Paden::Start()
{
	__super::Start();

	for (auto& elem : m_vecAdjPlayers)
	{
		DISABLE_GAMEOBJECT(elem->Get_CurrentUnit());
	}

	return S_OK;
}
