#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
END

/* ���� ������ ��� �ִ� Ŭ���� */
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

	//2. ���� ������ ������ �˷��־���


};

END