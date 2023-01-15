#include "CTrigger_Glider.h"
#include "UsefulHeaders.h"

CTrigger_Glider::CTrigger_Glider()
{
}

CTrigger_Glider::~CTrigger_Glider()
{
}

CTrigger_Glider* CTrigger_Glider::Create(string strPositionKey, _float fRadius)
{
	CTrigger_Glider* pInstance = new CTrigger_Glider;

	pInstance->m_strTriggerName = strPositionKey;
	pInstance->m_vPosition = CGameSystem::Get_Instance()->Find_Position(strPositionKey);
	pInstance->m_pTransform->Set_World(WORLD_POS, pInstance->m_vPosition);
	pInstance->m_pTransform->Make_WorldMatrix();
	pInstance->m_fRadius = fRadius;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CTrigger_Glider");
		SAFE_DELETE(pInstance);
	}
	

	return pInstance;
}

void CTrigger_Glider::Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
}

void CTrigger_Glider::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger_Glider::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}
