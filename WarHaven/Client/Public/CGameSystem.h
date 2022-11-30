#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CPositionTable;
class CPlayer;
class CPlayerInfo;
class CTrigger;
class CTeamConnector;

class CGameSystem
{
	DECLARE_SINGLETON(CGameSystem)

private:
	CGameSystem();
	~CGameSystem();

public:
	HRESULT					Initialize();
	HRESULT					Tick();
	void					Release();

public:
	HRESULT					On_ExitLevel();

public:
	HRESULT					On_ReadyTest(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

public: /* BootCamp */
	HRESULT					On_ReadyBootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

	HRESULT					On_ReadyPlayers_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyUIs_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyTriggers_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyDestructible_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

	HRESULT					On_EnterBootCamp();

public:	// Paden
	HRESULT					On_ReadyPaden(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

public: /* Position Table */
	HRESULT					Load_Position(string strFileKey);
	_float4					Find_Position(string strPositionKey);
	void					Add_Position(string strPositionKey, _float4 vPosition);

private:
	CPositionTable* m_pPositionTable = nullptr;

private:
	/* �������� ���Խ� ���� ��������� */
	CTeamConnector* m_pRedTeam = nullptr;
	CTeamConnector* m_pBlueTeam = nullptr;

private:
	/* ��� �÷��̾� ������ �̸� �������� �� ������ ���� Player �����ϴ� ��� */
	map<_hashcode, CPlayerInfo*>	m_mapAllPlayers;

private:
	/* Ʈ���� (����)�� map���� ��� �ֱ�. */
	map<_hashcode, CTrigger*>	m_mapAllTriggers;


private:
	HRESULT					SetUp_AllPlayerInfos();

private:
	CPlayer*				SetUp_Player(_hashcode hcPlayerInfo);
	HRESULT					SetUp_DefaultLight_BootCamp();


};

END