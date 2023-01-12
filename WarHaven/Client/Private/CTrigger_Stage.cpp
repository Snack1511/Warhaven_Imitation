#include "stdafx.h"
#include "CTrigger_Stage.h"

#include "CGameSystem.h"

#include "UsefulHeaders.h"
#include "CTeamConnector.h"
#include "CPlayer.h"

#include "CDominion_Effect.h"
#include "CUI_Popup.h"
CTrigger_Stage::CTrigger_Stage()
{
}

CTrigger_Stage::~CTrigger_Stage()
{
}

void CTrigger_Stage::Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	m_eTeamType = (eTEAM_TYPE)eOtherColType;

	//1. �浹�� ����� ��� ������ üũ
	if (eOtherColType == COL_BLUETEAM)
	{
		++m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE];

		_bool bIsMainPlayer = static_cast<CUnit*>(pOtherObj)->Is_MainPlayer();
		if (bIsMainPlayer)
			CUser::Get_Instance()->Move_PointUI(m_strTriggerName, 0);

	}
	else if (eOtherColType == COL_REDTEAM)
	{
		++m_iTeamCnt[(_uint)eTEAM_TYPE::eRED];

		_bool bIsMainPlayer = static_cast<CUnit*>(pOtherObj)->Is_MainPlayer();
		if (bIsMainPlayer)
			CUser::Get_Instance()->Move_PointUI(m_strTriggerName, 0);
	}

}

void CTrigger_Stage::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger_Stage::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	//1. �浹�� ����� �÷��̾������� üũ
	if (eOtherColType == COL_BLUETEAM)
	{
		--m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE];

		_bool bIsMainPlayer = static_cast<CUnit*>(pOtherObj)->Is_MainPlayer();
		if (bIsMainPlayer)
			CUser::Get_Instance()->Move_PointUI(m_strTriggerName, 1);
	}
	else if (eOtherColType == COL_REDTEAM)
	{
		--m_iTeamCnt[(_uint)eTEAM_TYPE::eRED];

		_bool bIsMainPlayer = static_cast<CUnit*>(pOtherObj)->Is_MainPlayer();
		if (bIsMainPlayer)
			CUser::Get_Instance()->Move_PointUI(m_strTriggerName, 1);
	}
}

CTrigger_Stage* CTrigger_Stage::Create(string strPositionKey, _float fRadius, eSTAGE_TRIGGER_TYPE eEnum)
{
	CTrigger_Stage* pInstance = new CTrigger_Stage;

	pInstance->m_eTriggerType = eEnum;
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

_float4 CTrigger_Stage::Get_RespawnPosition()
{
	_float4 vPos = m_vRespawnPositions.front();
	m_vRespawnPositions.pop_front();
	m_vRespawnPositions.push_back(vPos);
	return vPos;
}

HRESULT CTrigger_Stage::Initialize_Prototype()
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

HRESULT CTrigger_Stage::Start()
{
	__super::Start();

	return S_OK;
}

void CTrigger_Stage::My_Tick()
{
	if (m_eTriggerType == eSTAGE_TRIGGER_TYPE::eSTART)
		return;

	CUser::Get_Instance()->Set_ConquestTime(m_strTriggerName, m_fConqueredTimeAcc, m_fConqueredTime);

	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	_bool isIsFrustum = GAMEINSTANCE->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.1f);
	switch (m_eTriggerType)
	{
	case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eMAIN:
		CUser::Get_Instance()->Set_PointUI_ProjectionTransform(0, m_pTransform, isIsFrustum);
		CUser::Get_Instance()->Set_MiniMapConquestTime(0, m_fConqueredTimeAcc, m_fConqueredTime);
		CUser::Get_Instance()->Set_ScoreBoardConquestTime(0, m_fConqueredTimeAcc, m_fConqueredTime);
		break;

	case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eRESPAWN:
		CUser::Get_Instance()->Set_PointUI_ProjectionTransform(1, m_pTransform, isIsFrustum);
		CUser::Get_Instance()->Set_MiniMapConquestTime(1, m_fConqueredTimeAcc, m_fConqueredTime);
		CUser::Get_Instance()->Set_ScoreBoardConquestTime(1, m_fConqueredTimeAcc, m_fConqueredTime);
		break;

	case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eCANNON:
		CUser::Get_Instance()->Set_PointUI_ProjectionTransform(2, m_pTransform, isIsFrustum);
		CUser::Get_Instance()->Set_MiniMapConquestTime(2, m_fConqueredTimeAcc, m_fConqueredTime);
		CUser::Get_Instance()->Set_ScoreBoardConquestTime(2, m_fConqueredTimeAcc, m_fConqueredTime);
		break;

	case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eHWARA_CENTER:
		CUser::Get_Instance()->Set_PointUI_ProjectionTransform(0, m_pTransform, isIsFrustum);
		CUser::Get_Instance()->Set_MiniMapConquestTime(0, m_fConqueredTimeAcc, m_fConqueredTime);
		CUser::Get_Instance()->Set_ScoreBoardConquestTime(0, m_fConqueredTimeAcc, m_fConqueredTime);
		break;
	}

	//1. �� �� ���ʸ� 0�� ��
	if (m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE] != m_iTeamCnt[(_uint)eTEAM_TYPE::eRED])
	{
		if (m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE] == 0 || m_iTeamCnt[(_uint)eTEAM_TYPE::eRED] == 0)
		{
			/* ���� �ð� ���� */
			Update_Conquered();
			return;
		}
	}

	m_bUpdateStart = true;

	//2. �� �ܿ��� �ð� �ٱ�
	if (m_fConqueredTimeAcc > 0.f)
		m_fConqueredTimeAcc -= fDT(0);
	else
		m_fConqueredTimeAcc = 0.f;
}

void CTrigger_Stage::Update_Conquered()
{
	//�̹� ���ɴ��� �����̸�
	if (m_pConqueredTeam)
	{
		//���ɴ��� ���� ����� �Ѹ� �̶� ������ ����
		if (m_iTeamCnt[(_uint)m_pConqueredTeam->Get_TeamType()] > 0)
			return;
	}

	if (m_bUpdateStart)
	{
		m_bUpdateStart = false;

		/* blue�������� red�� ������ �˾ƾ��� */
		_bool bMainPlayerTeam = false;
		if (m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE] > m_iTeamCnt[(_uint)eTEAM_TYPE::eRED])
			bMainPlayerTeam = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eBLUE)->IsMainPlayerTeam();
		else
			bMainPlayerTeam = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eRED)->IsMainPlayerTeam();

		CUser::Get_Instance()->Interat_PointUI(bMainPlayerTeam, m_strTriggerName);

		switch (m_eTriggerType)
		{
		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eMAIN:
			CUser::Get_Instance()->Set_MiniMapGaugeColor(bMainPlayerTeam, 0);
			CUser::Get_Instance()->Set_ScoreBoardGaugeColor(bMainPlayerTeam, 0);
			break;
		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eRESPAWN:
			CUser::Get_Instance()->Set_MiniMapGaugeColor(bMainPlayerTeam, 1);
			CUser::Get_Instance()->Set_ScoreBoardGaugeColor(bMainPlayerTeam, 1);
			break;
		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eCANNON:
			CUser::Get_Instance()->Set_MiniMapGaugeColor(bMainPlayerTeam, 2);
			CUser::Get_Instance()->Set_ScoreBoardGaugeColor(bMainPlayerTeam, 2);
			break;

		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eHWARA_CENTER:
			CUser::Get_Instance()->Set_MiniMapGaugeColor(bMainPlayerTeam, 0);
			CUser::Get_Instance()->Set_ScoreBoardGaugeColor(bMainPlayerTeam, 0);
			break;
		}

	}

	_float fConquerSpeed = max(m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE], m_iTeamCnt[(_uint)eTEAM_TYPE::eRED]) * 0.5f;
	m_fConqueredTimeAcc += fDT(0) * fConquerSpeed;

#ifdef _DEBUG

	//cout << m_strTriggerName << " : " << m_fConqueredTimeAcc << endl;

#endif // _DEBUG

	if (m_fConqueredTimeAcc >= m_fConqueredTime)
	{
		// ���ɴ��� ���� ���� ������ �� ������ ����

		//���� ������ �־����� �ű⼭ trigger ����
		if (m_pConqueredTeam)
			m_pConqueredTeam->Erase_Trigger(m_strTriggerName);

		//�� ���� ���� ������ �ȴ�.
		if (m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE] > m_iTeamCnt[(_uint)eTEAM_TYPE::eRED])
			m_pConqueredTeam = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eBLUE);
		else
			m_pConqueredTeam = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eRED);

		m_pDominionEffect->Set_DominionColor(m_pConqueredTeam);

		_bool IsMainPlayerTeam = m_pConqueredTeam->IsMainPlayerTeam();
		if (IsMainPlayerTeam)
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eConquest);
		}

		CUser::Get_Instance()->Conquest_PointUI(m_strTriggerName, IsMainPlayerTeam);

		switch (m_eTriggerType)
		{
		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eMAIN:
			CUser::Get_Instance()->Set_MiniMapPointColor(IsMainPlayerTeam, 0);
			CUser::Get_Instance()->Set_OperPointColor(IsMainPlayerTeam, 0);
			CUser::Get_Instance()->Set_ScoreBoardPointColor(IsMainPlayerTeam, 0);
			break;
		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eRESPAWN:
			CUser::Get_Instance()->Set_MiniMapPointColor(IsMainPlayerTeam, 1);
			CUser::Get_Instance()->Set_OperPointColor(IsMainPlayerTeam, 1);
			CUser::Get_Instance()->Set_ScoreBoardPointColor(IsMainPlayerTeam, 1);
			break;
		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eCANNON:
			CUser::Get_Instance()->Set_MiniMapPointColor(IsMainPlayerTeam, 2);
			CUser::Get_Instance()->Set_OperPointColor(IsMainPlayerTeam, 2);
			CUser::Get_Instance()->Set_ScoreBoardPointColor(IsMainPlayerTeam, 2);
			break;

		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eHWARA_CENTER:
		
			/* Center ���� �� final ���� �����ϰ� ���� */
			CGameSystem::Get_Instance()->Enable_HwaraFinalTrigger(m_pConqueredTeam->Get_TeamType());

			CUser::Get_Instance()->Set_MiniMapPointColor(IsMainPlayerTeam, 0);
			CUser::Get_Instance()->Set_OperPointColor(IsMainPlayerTeam, 0);
			CUser::Get_Instance()->Set_ScoreBoardPointColor(IsMainPlayerTeam, 0);

			break;

		case Client::CTrigger_Stage::eSTAGE_TRIGGER_TYPE::eHWARA_FINAL:
			/* Final ���� �� ���� ���� */
			if (m_pConqueredTeam == CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eBLUE))
				CGameSystem::Get_Instance()->On_FinishGame(CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eRED));
			else
				CGameSystem::Get_Instance()->On_FinishGame(CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eBLUE));

			break;

		}



		m_pConqueredTeam->Add_Trigger(this);



#ifdef _DEBUG

		string strName = "BLUE";

		if (m_pConqueredTeam->Get_TeamType() == eTEAM_TYPE::eRED)
			strName = "RED";

		cout << m_strTriggerName << " : ���� �Ϸ� by " << strName << endl;

		if (m_pConqueredTeam->IsMainPlayerTeam())

			cout << m_strTriggerName << " �����÷��̾� �� ���� " << endl;

#endif // _DEBUG

		m_fConqueredTimeAcc = 0.f;
	}

}
