#pragma once
#include "Client_Defines.h"

#define		MAX_PLAYERSNUM 16

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CPositionTable;
class CPlayer;
class CPlayerInfo;
class CTrigger;
class CTeamConnector;
class CAIPersonality;
class CTable_Conditions;
class CBehavior;
class CPath;
class CAIController;

class CGameSystem
{
public:
	enum eSTAGE_TYPE {eSTAGE_PADEN, eSTAGE_CNT};
	DECLARE_SINGLETON(CGameSystem)

private:
	CGameSystem();
	~CGameSystem();

	friend class CWindow_Path;

public:
	HRESULT					Initialize();
	HRESULT					Tick();
	void					Release();

public:
	HRESULT					On_ExitLevel();

public:
	HRESULT					On_ReadyTest(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_EnterTest();

public: /* BootCamp */
	HRESULT					On_ReadyBootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

	HRESULT					On_ReadyPlayers_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyUIs_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyTriggers_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyDestructible_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

	HRESULT					On_EnterBootCamp();

	HRESULT					BootCamp_EnvironmentEffect();

public:	// Paden
	HRESULT					On_ReadyPaden(vector<pair<CGameObject*, _uint>>& vecReadyObjects);

	/* �������� �÷��̾� ���� */
	HRESULT					On_ReadyPlayers_Stage(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	
	/* �ĵ� �������� ���� ���� */
	HRESULT					On_ReadyTirggers_Paden(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyDestructible_Paden(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_EnterStage();
	HRESULT					On_Update_Paden();

	HRESULT					Paden_EnvironmentEffect();

	/* ����ȸ�� ������ ��� ������Ű�� */
	void					On_StartGame();
	void					On_FinishGame();

	CTeamConnector* Get_Team(eTEAM_TYPE eEnum) { return m_pTeamConnector[(_uint)eEnum]; }

public: /* Position Table */
	HRESULT					Load_Position(string strFileKey);
	_float4					Find_Position(string strPositionKey);
	void					Add_Position(string strPositionKey, _float4 vPosition);

public: /* Behavior Table */
	CTable_Conditions* Get_BXTable() { return m_pConditionTable; }
	CBehavior*		Clone_Behavior(wstring wstrBXKey);

public: /* Pathes */
	CPath*			Find_Path(string strPathKey);
	CPath*			Clone_Path(string strPathKey, CAIController* pOwnerController);

	CPath*			Clone_RandomStartPath(CAIController* pOwnerController, eTEAM_TYPE eTeamType);

public:	
	CTrigger*					Find_Trigger(string strTriggerKey);

public:
	CPlayerInfo* Find_PlayerInfo(_hashcode hcCode);
	eSTAGE_TYPE m_eCurStageType = eSTAGE_TYPE::eSTAGE_CNT;
private:
	CPositionTable* m_pPositionTable = nullptr;
	CTable_Conditions* m_pConditionTable = nullptr;

private:
	/* �������� ���Խ� ���� ��������� */
	CTeamConnector* m_pTeamConnector[_uint(eTEAM_TYPE::eCOUNT)] = {};
	_float			m_fScoreAcc = 0.f;
	_float			m_fScoreMinusTime = 1.5f;

private:
	/* ��� �÷��̾� ������ �̸� �������� �� ������ ���� Player �����ϴ� ��� */
	map<_hashcode, CPlayerInfo*>	m_mapAllPlayers;

private:
	/* Ʈ���� (����)�� map���� ��� �ֱ�. */
	map<_hashcode, CTrigger*>	m_mapAllTriggers;

private:
	/* Path�� map���� ��� �ֱ�. */
	map<_hashcode, CPath*>	m_mapAllPathes[eSTAGE_CNT];

private:
	HRESULT					SetUp_AllPlayerInfos();
	HRESULT					SetUp_AllPathes();

private:
	CPlayer*				SetUp_Player(_hashcode hcPlayerInfo);
	HRESULT					SetUp_DefaultLight_BootCamp();
	HRESULT					SetUp_DefaultLight_Paden();

private:



};

END