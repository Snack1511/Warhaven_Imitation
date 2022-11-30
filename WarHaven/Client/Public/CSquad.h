#pragma once
#include "Client_Defines.h"

#define		MAX_SQUAD 4

BEGIN(Engine)
END

/* 본인 스쿼드 플레이어들을 들고 있음 */
BEGIN(Client)
class CPlayer;
class CSquad
{
	friend class CTeamConnector;

private:
	CSquad();
	~CSquad();


public:
	void		Add_Player(CPlayer* pPlayer);
	CPlayer*	Find_Player(wstring wstrName);
	void		Make_LeaderPlayer();
	void		Clear_All() { m_pLeaderPlayer = nullptr; m_mapPlayers.clear(); }


private:
	CPlayer* m_pLeaderPlayer = nullptr;

	//1. 본인팀 플레이어를 다 들고 있음. key값 : 플레이어 이름
	map<_hashcode, CPlayer*>	m_mapPlayers;



};

END