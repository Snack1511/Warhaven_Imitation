#include "stdafx.h"
#include "CTrigger_Paden.h"

#include "CGameSystem.h"

#include "UsefulHeaders.h"
#include "CTeamConnector.h"
#include "CPlayer.h"

#include "CDominion_Effect.h"

CTrigger_Paden::CTrigger_Paden()
{
}

CTrigger_Paden::~CTrigger_Paden()
{
}

void CTrigger_Paden::Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	m_eTeamType = (eTEAM_TYPE)eOtherColType;

	//1. 충돌한 대상이 어느 팀인지 체크
	if (eOtherColType == COL_BLUETEAM)
	{
		++m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE];
	}
	else if (eOtherColType == COL_REDTEAM)
	{
		++m_iTeamCnt[(_uint)eTEAM_TYPE::eRED];
	}

	// 상단 유아이 하이라이트
	CUser::Get_Instance()->Interat_PointUI(m_strTriggerName, eOtherColType, 0);
}

void CTrigger_Paden::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger_Paden::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	//1. 충돌한 대상이 플레이어팀인지 체크
	if (eOtherColType == COL_BLUETEAM)
	{
		--m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE];
	}
	else if (eOtherColType == COL_REDTEAM)
	{
		--m_iTeamCnt[(_uint)eTEAM_TYPE::eRED];
	}

	// 상단 유아이 원위치
	CUser::Get_Instance()->Interat_PointUI(m_strTriggerName, 99, 1);
}

CTrigger_Paden* CTrigger_Paden::Create(string strPositionKey, _float fRadius, ePADEN_TRIGGER_TYPE eEnum)
{
	CTrigger_Paden* pInstance = new CTrigger_Paden;

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
	if (m_eTriggerType == ePADEN_TRIGGER_TYPE::eSTART)
		return;

	CUser::Get_Instance()->Set_ConquestTime(m_strTriggerName, m_fConqueredTimeAcc, m_fConqueredTime);

	//1. 둘 중 한쪽만 0일 때
	if (m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE] != m_iTeamCnt[(_uint)eTEAM_TYPE::eRED])
	{
		if (m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE] == 0 || m_iTeamCnt[(_uint)eTEAM_TYPE::eRED] == 0)
		{
			/* 점령 시간 차기 */
			Update_Conquered();
			return;
		}
	}


	//2. 그 외에는 시간 줄기
	if (m_fConqueredTimeAcc > 0.f)
		m_fConqueredTimeAcc -= fDT(0);
	else
		m_fConqueredTimeAcc = 0.f;

	// 플레이어와 거리가 제일 가까운 트리거만
	// 혹은 목표로 지정한 트리거만 나침반으로 표시

	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	_bool isIsFrustum = GAMEINSTANCE->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.1f);

	if (m_strTriggerName == "Paden_Trigger_A")
	{
		CUser::Get_Instance()->Set_PointUI_ProjectionTransform(0, m_pTransform, isIsFrustum);
	}
	else if (m_strTriggerName == "Paden_Trigger_R")
	{
		CUser::Get_Instance()->Set_PointUI_ProjectionTransform(1, m_pTransform, isIsFrustum);
	}
	else if (m_strTriggerName == "Paden_Trigger_C")
	{
		CUser::Get_Instance()->Set_PointUI_ProjectionTransform(2, m_pTransform, isIsFrustum);
	}
}

void CTrigger_Paden::Update_Conquered()
{
	//이미 점령당한 거점이면
	if (m_pConqueredTeam)
	{
		//점령당한 팀쪽 사람이 한명 이라도 있으면 리턴
		if (m_iTeamCnt[(_uint)m_pConqueredTeam->Get_TeamType()] > 0)
			return;
	}

	_float fConquerSpeed = max(m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE], m_iTeamCnt[(_uint)eTEAM_TYPE::eRED]) * 0.5f;
	m_fConqueredTimeAcc += fDT(0) * fConquerSpeed;

#ifdef _DEBUG

	//cout << m_strTriggerName << " : " << m_fConqueredTimeAcc << endl;

#endif // _DEBUG

	if (m_fConqueredTimeAcc >= m_fConqueredTime)
	{
		// 점령당한 거점 색깔 점령한 팀 색으로 변경
		CUser::Get_Instance()->Conquest_PointUI(m_strTriggerName, (_uint)m_eTeamType);

		//이전 주인이 있었으면 거기서 trigger 빼기
		if (m_pConqueredTeam)
			m_pConqueredTeam->Erase_Trigger(m_strTriggerName);

		//더 많은 쪽의 소유가 된다.
		if (m_iTeamCnt[(_uint)eTEAM_TYPE::eBLUE] > m_iTeamCnt[(_uint)eTEAM_TYPE::eRED])
			m_pConqueredTeam = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eBLUE);
		else
			m_pConqueredTeam = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eRED);

		m_pDominionEffect->Set_DominionColor(m_pConqueredTeam);

		m_pConqueredTeam->Add_Trigger(this);

#ifdef _DEBUG

		string strName = "BLUE";

		if (m_pConqueredTeam->Get_TeamType() == eTEAM_TYPE::eRED)
			strName = "RED";

		cout << m_strTriggerName << " : 점령 완료 by " << strName << endl;

		if (m_pConqueredTeam->IsMainPlayerTeam())

			cout << m_strTriggerName << " 메인플레이어 팀 점령 " << endl;

#endif // _DEBUG

		m_fConqueredTimeAcc = 0.f;
	}

}
