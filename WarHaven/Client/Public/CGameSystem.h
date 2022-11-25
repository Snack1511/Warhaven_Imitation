#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CPositionTable;
class CPlayer;

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

public: /* BootCamp */
	HRESULT					On_ReadyBootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyPlayers(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyUIs(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyTriggers(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

	HRESULT					On_EnterBootCamp();

public: /* Position Table */
	HRESULT					Load_Position(string strFileKey);
	_float4					Find_Position(string strPositionKey);
	void					Add_Position(string strPositionKey, _float4 vPosition);

private:
	CPositionTable* m_pPositionTable = nullptr;

private:
	map<string, CPlayer*>	m_mapEnemyPlayers;

private:
	CPlayer* SetUp_Player(_float4 vStartPos, _uint iClassType, STATE_TYPE eStartState, _bool bUserPlayer, wstring wstrCamName);

};

END