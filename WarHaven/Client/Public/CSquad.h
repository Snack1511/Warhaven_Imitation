#pragma once
#include "Client_Defines.h"

#define		MAX_SQUAD 4

BEGIN(Engine)
END

/* 본인 스쿼드 플레이어들을 들고 있음 */
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

	//툴 전용 생성 함수(CTeamConnector->Add_Squad())에서 사용하는 함수
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
public:/* 툴 전용 생성 삭제..*/
	void Add_EmptyPlayer(_bool bLeader, CPlayer* pPlayer);
	void Delete_EmptyPlayer(CPlayer* pPlayer);
private:
	CPlayer* m_pLeaderPlayer = nullptr;

	//1. 본인팀 플레이어를 다 들고 있음. key값 : 플레이어 이름
	map<_hashcode, CPlayer*>	m_mapPlayers;

	CTeamConnector* m_pMyTeam = nullptr;



};

END