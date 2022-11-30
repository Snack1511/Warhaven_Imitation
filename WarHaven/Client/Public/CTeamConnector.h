#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
END

/* 팀의 정보를 담고 있는 클래스 */
BEGIN(Client)

class CPlayer;
class CSquad;

class CTeamConnector
{

private:
	CTeamConnector();
	~CTeamConnector();


public:
	CPlayer* Find_Player(wstring wstrName);

public:
	void	Release();

private:
	list<CSquad*>	m_SquadList;

	//2. 팀이 점령한 거점도 알려주야함


};

END