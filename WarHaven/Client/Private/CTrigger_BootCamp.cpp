#include "stdafx.h"
#include "CTrigger_BootCamp.h"

#include "CGameSystem.h"

#include "UsefulHeaders.h"

#include "CPlayer.h"

CTrigger_BootCamp::CTrigger_BootCamp()
{
}

CTrigger_BootCamp::~CTrigger_BootCamp()
{
}

void CTrigger_BootCamp::Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	//1. 충돌한 대상이 플레이어인지 체크
	if (eOtherColType == COL_BLUETEAM)
	{
		if (static_cast<CUnit*>(pOtherObj)->Is_MainPlayer())
		{
			if (m_iUIIndex == 100)
			{
				for (auto& elem : m_vecAdjPlayers)
				{
					ENABLE_GAMEOBJECT(elem->Get_CurrentUnit());
					elem->Set_LookToTarget();
				}
				m_vecAdjPlayers.clear();

			}
			else if (m_iUIIndex == 99)
			{				
				if (!m_vecAdjTriggers.empty())
				{
					for (int i = 0;i < m_vecAdjTriggers.size(); ++i)
					{
						DISABLE_GAMEOBJECT(m_vecAdjTriggers[i]);
					}

					ENABLE_GAMEOBJECT(m_vecAdjTriggers.back());
				}
			}
			else
			{
				CUser::Get_Instance()->SetActive_TrainingPopup(true, m_iUIIndex);
				
				for (auto& elem : m_vecAdjPlayers)
				{
					ENABLE_GAMEOBJECT(elem->Get_CurrentUnit());
					elem->Set_LookToTarget();
				}

				m_vecAdjPlayers.clear();
			}
		}
	}


}

void CTrigger_BootCamp::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger_BootCamp::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	//1. 충돌한 대상이 플레이어인지 체크
	if (eOtherColType == COL_BLUETEAM)
	{
		if (static_cast<CUnit*>(pOtherObj)->Is_MainPlayer())
		{
			CUser::Get_Instance()->SetActive_TrainingPopup(false, m_iUIIndex);
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

HRESULT CTrigger_BootCamp::Start()
{
	__super::Start();

	/*for (auto& elem : m_vecAdjPlayers)
	{
		DISABLE_GAMEOBJECT(elem->Get_CurrentUnit());
	}*/

	return S_OK;
}
