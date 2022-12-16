#pragma once
#include "Client_Defines.h"

#define		MAX_SQUAD 4

BEGIN(Engine)
END

/* ���� ������ �÷��̾���� ��� ���� */
BEGIN(Client)
class CPlayer;
class CTeamConnector;
class CSquad
{
	friend class CTeamConnector;
	friend class CGameSystem;

private:
	CSquad();
	~CSquad();

public:
	static CSquad* Create(CPlayer* pLeaderPlayer, CPlayer* pPlayer_0
		, CPlayer* pPlayer_1, CPlayer* pPlayer_2);

	//�� ���� ���� �Լ�(CTeamConnector->Add_Squad())���� ����ϴ� �Լ�
	static CSquad* Create(CTeamConnector* pConnector);

public:
	HRESULT	Initialize();


public:
	void		Add_Player(CPlayer* pPlayer);
	CPlayer*	Find_Player(wstring wstrName);
	void		Make_LeaderPlayer();
	void		Clear_All() { m_pLeaderPlayer = nullptr; m_mapPlayers.clear(); }
	void	SetUp_OutlineType_SquadMember();
	CPlayer* Get_LeaderPlayer() { return m_pLeaderPlayer; }

	//map<_hashcode, CPlayer*>	Get_AllPlayers() { return m_mapPlayers; };
	map<_hashcode, CPlayer*>&	Get_AllPlayers() { return m_mapPlayers; };
public:/* �� ���� ���� ����..*/
	void Add_EmptyPlayer(_bool bLeader, CPlayer* pPlayer);
	void Delete_EmptyPlayer(CPlayer* pPlayer);
private:
	CPlayer* m_pLeaderPlayer = nullptr;

	//1. ������ �÷��̾ �� ��� ����. key�� : �÷��̾� �̸�
	map<_hashcode, CPlayer*>	m_mapPlayers;

	CTeamConnector* m_pMyTeam = nullptr;



};

END