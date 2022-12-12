#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
END

/* 팀의 정보를 담고 있는 클래스 */
BEGIN(Client)

class CPlayer;
class CSquad;
class CTrigger;

class CTeamConnector
{
	friend class CGameSystem;

private:
	CTeamConnector();
	~CTeamConnector();

public:
	static CTeamConnector* Create(list<CSquad*> SquadList);

public:
	HRESULT	Initialize();
	/* 팀 다 배치한 이후, mainplayer 기준으로 피아식별 하기 */
	void	SetUp_TeamType();

public:
	CPlayer* Find_Player(wstring wstrName);
	
public:
	void	Add_Trigger(CTrigger* pTrigger);
	void	Erase_Trigger(string strTriggerKey);

	_float4	Find_RespawnPosition_Start();
	_float4	Find_RespawnPosition(string strTriggerKey);
	
public:
	eTEAM_TYPE	Get_TeamType() { return m_eTeamType; }
	_bool	IsMainPlayerTeam() { return m_bIsMainPlayerTeam; }
	_bool	Minus_Score();
	_bool	Has_MainTrigger() { return m_bHasMainTrigger; }

public:
	/* 파덴 진입시 호출 */
	HRESULT	On_EnterPaden();

public:
	void	Release();

private:
	eTEAM_TYPE		m_eTeamType = eTEAM_TYPE::eBLUE;
	_bool			m_bIsMainPlayerTeam = false;
	list<CSquad*>	m_SquadList;

	//2. 팀이 점령한 거점도 알려주야함
	list<CTrigger*>	m_OurTriggers;
	_bool			m_bHasMainTrigger = false;

private:
	//3. 남은 점수 표기
	_uint			m_iScore = 0;
	_uint			m_iMaxScore = 3;
};

END