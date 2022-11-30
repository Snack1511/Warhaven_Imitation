#pragma once
#include "Client_Defines.h"

#define		MAX_SQUAD 4

BEGIN(Engine)
END

/* ���� ������ �÷��̾���� ��� ���� */
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

	//1. ������ �÷��̾ �� ��� ����. key�� : �÷��̾� �̸�
	map<_hashcode, CPlayer*>	m_mapPlayers;



};

END