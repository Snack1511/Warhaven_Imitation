#include "stdafx.h"
#include "CSquad.h"

#include "UsefulHeaders.h"



CSquad::CSquad()
{
}

CSquad::~CSquad()
{
}

CSquad* CSquad::Create(CPlayer* pLeaderPlayer, CPlayer* pPlayer_0, CPlayer* pPlayer_1, CPlayer* pPlayer_2)
{
	CSquad* pSquad = new CSquad;

	pSquad->m_pLeaderPlayer = pLeaderPlayer;
	pSquad->Add_Player(pLeaderPlayer);
	pSquad->Add_Player(pPlayer_0);
	pSquad->Add_Player(pPlayer_1);
	pSquad->Add_Player(pPlayer_2);

	if (pSquad->m_mapPlayers.size() < 4)
	{
		Call_MsgBox(L"스쿼드에 멤버 부족 : CSquad");
		SAFE_DELETE(pSquad);
	}

	return pSquad;
}

CSquad* CSquad::Create(CTeamConnector* pConnector)
{
	CSquad* pSquad = new CSquad;
	pSquad->m_pMyTeam = pConnector;

	return pSquad;
}

HRESULT CSquad::Initialize()
{
	if (!m_pMyTeam)
		return E_FAIL;

	for (auto& elem : m_mapPlayers)
	{
		elem.second->Set_Squad(this);
		elem.second->Set_Team(m_pMyTeam);
	}

	m_pLeaderPlayer->Set_LeaderPlayer();

	return S_OK;
}

void CSquad::Add_Player(CPlayer* pPlayer)
{
	if (m_mapPlayers.size() >= MAX_SQUAD)
		return;

	if (Find_Player(pPlayer->Get_PlayerName()))
		return;

	m_mapPlayers.emplace(Convert_ToHash(pPlayer->Get_PlayerName()), pPlayer);
}

CPlayer* CSquad::Find_Player(wstring wstrName)
{
	auto iter = m_mapPlayers.find(Convert_ToHash(wstrName));
	if (iter == m_mapPlayers.end())
		return nullptr;

	return iter->second;
}

void CSquad::Make_LeaderPlayer()
{
	if (m_mapPlayers.size() < MAX_SQUAD)
	{
		Call_MsgBox_Index(L"Squad에 플레이어 부족 / 현재 인원 : ", m_mapPlayers.size());
		return;
	}

	_uint iMaxLevel = 0;
	CPlayer* pPlayer = nullptr;
	for (auto& elem : m_mapPlayers)
	{
		if (iMaxLevel < elem.second->Get_Level())
		{
			iMaxLevel = elem.second->Get_Level();
			pPlayer = elem.second;
		}
	}

	if (pPlayer)
		m_pLeaderPlayer = pPlayer;
}

void CSquad::SetUp_OutlineType_SquadMember()
{
	for (auto& elem : m_mapPlayers)
	{
		if (elem.second == m_pLeaderPlayer)
			continue;
		elem.second->Set_OutlineType(CPlayer::OUTLINETYPE::eSQUADMEMBER);
	}
}

void CSquad::Add_EmptyPlayer(_bool bLeader, CPlayer* pPlayer)
{
	if (nullptr == pPlayer)
		return;
	if (bLeader)
	{
		pPlayer->Set_LeaderPlayer();
		m_pLeaderPlayer = pPlayer;
	}
	pPlayer->Set_Squad(this);
	pPlayer->Set_Team(m_pMyTeam);
	Add_Player(pPlayer);
}

void CSquad::Delete_EmptyPlayer(CPlayer* pPlayer)
{
	if (nullptr == pPlayer)
		return;
	wstring wstrName = pPlayer->Get_PlayerName();
	auto iter = m_mapPlayers.find(Convert_ToHash(wstrName));
	if (iter == m_mapPlayers.end())
		return;
	else m_mapPlayers.erase(iter);

	if (pPlayer->IsLeaderPlayer())
	{
		_uint iMaxLevel = 0;
		CPlayer* pTmpPlayer = nullptr;
		for (auto& elem : m_mapPlayers)
		{
			if (iMaxLevel < elem.second->Get_Level())
			{
				iMaxLevel = elem.second->Get_Level();
				pTmpPlayer = elem.second;
			}
		}
		if (nullptr != pTmpPlayer)
			pTmpPlayer->Set_LeaderPlayer();
		else
		{
			if (!m_mapPlayers.empty()) 
			{
				m_pLeaderPlayer = m_mapPlayers.begin()->second;
				m_mapPlayers.begin()->second->Set_LeaderPlayer();
			}
			else
			{
				m_pLeaderPlayer = nullptr;
			}
		}
	}
}
