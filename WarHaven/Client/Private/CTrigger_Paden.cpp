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
	//1. �浹�� ����� ��� ������ üũ
	if (eOtherColType == COL_PLAYERTEAM)
	{
		++m_iPlayerTeamCnt;
	}
	else if (eOtherColType == COL_ENEMYTEAM)
	{
		++m_iEnemyTeamCnt;

	}


}

void CTrigger_Paden::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger_Paden::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	//1. �浹�� ����� �÷��̾������� üũ
	if (eOtherColType == COL_PLAYERTEAM)
	{
		--m_iPlayerTeamCnt;
	}
	else if (eOtherColType == COL_ENEMYTEAM)
	{
		--m_iEnemyTeamCnt;

	}
}

CTrigger_Paden* CTrigger_Paden::Create(string strPositionKey, _float fRadius)
{
	CTrigger_Paden* pInstance = new CTrigger_Paden;

	pInstance->m_strTriggerName = strPositionKey;
	pInstance->m_vPosition = CGameSystem::Get_Instance()->Find_Position(strPositionKey);
	pInstance->m_pTransform->Set_World(WORLD_POS, pInstance->m_vPosition);
	pInstance->m_pTransform->Make_WorldMatrix();
	pInstance->m_fRadius = fRadius;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CTrigger_Paden");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CTrigger_Paden::Set_StartTrigger(_bool bPlayerTeam)
{
	m_bConquered = true;
	m_bIsConqueredByPlayerTeam = bPlayerTeam;
	m_bStartTrigger = true;
	
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

	_uint iIndex = 0;

	while (1)
	{
		string strPositionKey = m_strTriggerName;
		strPositionKey += "_";
		strPositionKey += to_string(iIndex++);

		_float4 vNewPos = CGameSystem::Get_Instance()->Find_Position(strPositionKey);
		if (vNewPos.Is_Zero())
			break;

		m_vRespawnPositions.push_back(vNewPos);
	}
	


	return __super::Initialize_Prototype();
}

HRESULT CTrigger_Paden::Start()
{
	__super::Start();


	return S_OK;
}

void CTrigger_Paden::My_Tick()
{
	//�� �� �ϳ��� ���� �� 
	if (m_bStartTrigger)
		return;

	if (m_iEnemyTeamCnt != m_iPlayerTeamCnt)
	{
		if (m_iEnemyTeamCnt == 0 || m_iPlayerTeamCnt == 0)
		{
			/* ���� �ð� ���� */
			Update_Conquered();


		}
	}

}

void CTrigger_Paden::Update_Conquered()
{
	//�̹� ���ɴ��� �����̸�
	if (m_bConquered)
	{
		//�÷��̾����� ���ɴ��� �����̸� 
		if (m_bIsConqueredByPlayerTeam)
		{
			//���༮��� á�� ���� ����
			if (m_iPlayerTeamCnt > 0)
				return;
		}
		else
		{
			if (m_iEnemyTeamCnt > 0)
				return;
		}
	}


	_float fConquerSpeed = max(m_iEnemyTeamCnt, m_iPlayerTeamCnt) * 0.5f;
	m_fConqueredTimeAcc += fDT(0) * fConquerSpeed;

	if (m_fConqueredTimeAcc >= m_fConqueredTime)
	{
		//�� ���� ���� ������ �ȴ�.
		m_bConquered = true;
		if (m_iEnemyTeamCnt > m_iPlayerTeamCnt)
			m_bIsConqueredByPlayerTeam = false;
		else
			m_bIsConqueredByPlayerTeam = true;

		m_fConqueredTimeAcc = 0.f;
	}

}
