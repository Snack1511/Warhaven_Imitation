#pragma once
#include "Client_Defines.h"

#define		MAX_PLAYERSNUM 16

BEGIN(Engine)
class CGameObject;
class CCellLayer;
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
class CCannon;

class CGameSystem
{
public:
	enum eSTAGE_TYPE {eSTAGE_PADEN, eSTAGE_HWARA, ePADEN_RELEASE, eHWARA_RELEASE, eSTAGE_CNT};
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

	/* 스테이지 플레이어 세팅 */
	HRESULT					On_ReadyPlayers_Stage(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	
	/* 파덴 스테이지 거점 세팅 */
	HRESULT					On_ReadyTriggers_Paden(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyDestructible_Paden(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_EnterStage();
	HRESULT					On_Update_Paden();

	HRESULT					Paden_EnvironmentEffect();

	/* 작전회의 끝나고 모두 생성시키기 */
	void					On_StartGame();
	void					On_FinishGame(CTeamConnector* pTeamConnector);

	CTeamConnector* Get_Team(eTEAM_TYPE eEnum) { return m_pTeamConnector[(_uint)eEnum]; }

public: /* Hwara */
	HRESULT					On_ReadyHwara(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyTriggers_Hwara(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_ReadyDestructible_Hwara(vector<pair<CGameObject*, _uint>>& vecReadyObjects);
	HRESULT					On_Update_Hwara();
	HRESULT					Hwara_EnvironmentEffect();


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
	CPath*			Clone_RandomRespawnPath(CAIController* pOwnerController, eTEAM_TYPE eTeamType);
	CPath*			Clone_CenterPath(CAIController* pOwnerController, eTEAM_TYPE eTeamType);

	CPath* Clone_RandomReleasePath(_float4 vCurPos);
	CPath* Clone_RandomNearestPath(_float4 vCurPos, CPlayer* pPlayer);


	CPath* Get_NearPath(_float4 vPosition);

public:
	CTrigger*					Find_Trigger(string strTriggerKey);
	void						Enable_HwaraFinalTrigger(eTEAM_TYPE eTeamType);

public:
	CPlayerInfo* Find_PlayerInfo(_hashcode hcCode);
	vector<wstring>* GetPtr_PlayerInfoNames() { return &m_vecPlayerInfoNames; }
	eSTAGE_TYPE m_eCurStageType = eSTAGE_TYPE::eSTAGE_CNT;

public:
	CCannon* Get_Cannon() { return m_pCannon; }

private:
	CCannon* m_pCannon = nullptr;

private:
	CPositionTable* m_pPositionTable = nullptr;
	CTable_Conditions* m_pConditionTable = nullptr;

private:
	/* 스테이지 진입시 팀이 만들어진다 */
	CTeamConnector* m_pTeamConnector[_uint(eTEAM_TYPE::eCOUNT)] = {};
	_float			m_fScoreAcc = 0.f;
	//_float			m_fScoreMinusTime = 100.5f;
	_float			m_fScoreMinusTime = 1.5f;

private:
	/* 모든 플레이어 정보를 미리 만들어놓고 그 정보를 토대로 Player 생성하는 방식 */
	map<_hashcode, CPlayerInfo*>	m_mapAllPlayers;
	vector<wstring>					m_vecPlayerInfoNames;
private:
	/* 트리거 (거점)을 map으로 들고 있기. */
	map<_hashcode, CTrigger*>	m_mapAllTriggers;

private:
	/* Path들 map으로 들고 있기. */
	map<_hashcode, CPath*>	m_mapAllPathes[eSTAGE_CNT];

private:
	HRESULT					SetUp_AllPlayerInfos();
	HRESULT					SetUp_AllPathes();
	
#pragma region Personality Functions
private:
	HRESULT					SetUp_AllPersonality();
	HRESULT					Reload_AllPersonality();
private:
	CAIPersonality* Add_Personality(string strHashKey, wstring wstrPath);
	CAIPersonality* Find_Personality(string strHashKey);
	void			Clear_AllPersonality();
	map<_hashcode, CAIPersonality*> m_mapPersonality;
#pragma endregion Personality Functions

private:
	CPlayer*				SetUp_Player(_hashcode hcPlayerInfo);
	HRESULT					SetUp_DefaultLight_BootCamp();
	HRESULT					SetUp_DefaultLight_Paden();

#pragma region CellLayer Functions
public:
	HRESULT					SetUp_CellLayer(wstring strFolderName);
	void					Clear_CellLayer();
	map<_float, CCellLayer*>& Get_CellLayer() { return m_CellLayer; }
	HRESULT					SetUp_DefaultLight_Hwara();

private:
	map<_float, CCellLayer*> m_CellLayer;
#pragma endregion CellLayer Functions

};

END