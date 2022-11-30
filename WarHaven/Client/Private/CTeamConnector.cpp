#include "stdafx.h"
#include "CTeamConnector.h"
#include "UsefulHeaders.h"
#include "CSquad.h"
CTeamConnector::CTeamConnector()
{
}

CTeamConnector::~CTeamConnector()
{
    Release();
}

CPlayer* CTeamConnector::Find_Player(wstring wstrName)
{
    CPlayer* pPlayer = nullptr;

    for (auto& elem : m_SquadList)
    {
        pPlayer = elem->Find_Player(wstrName);
    }

    return pPlayer;
}

void CTeamConnector::Release()
{
    for (auto& elem : m_SquadList)
        SAFE_DELETE(elem);
}
