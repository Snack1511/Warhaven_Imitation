#include "stdafx.h"
#include "CSquad.h"

#include "UsefulHeaders.h"

CSquad::CSquad()
{
}

CSquad::~CSquad()
{
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
