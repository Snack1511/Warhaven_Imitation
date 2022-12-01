#pragma once
#include "CTrigger.h"

BEGIN(Client)

class CPlayer;

class CTrigger_Paden 
	: public CTrigger
{
	DECLARE_PROTOTYPE(CTrigger_Paden);

protected:
	CTrigger_Paden();
	virtual ~CTrigger_Paden();

public:
	static CTrigger_Paden* Create(string strPositionKey, _float fRadius);

public:
	virtual void	Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	void		Add_AdjPlayer(CPlayer* pPlayer) { m_vecAdjPlayers.push_back(pPlayer); }
	void		Add_RespawnPositions(_float4 vPosition) { m_vRespawnPositions.push_back(vPosition); };

public:
	void		Set_StartTrigger(_bool bPlayerTeam);
	_float4		Get_RespawnPosition();
	string		Get_TriggerName() { return m_strTriggerName; }

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Start() override;

private:
	string			m_strTriggerName;

	/* 양 팀중 몇명이 트리거 안에 들어있는지 확인 */
	_uint			m_iPlayerTeamCnt = 0;
	_uint			m_iEnemyTeamCnt = 0;

	/* 리스폰 시에 이 포인트에서 front로 생성 */
	list<_float4>	m_vRespawnPositions;
	vector<CPlayer*> m_vecAdjPlayers;

private:
	_bool			m_bStartTrigger = false;
	_bool			m_bConquered = false;
	_bool			m_bIsConqueredByPlayerTeam = false;

	_float			m_fConqueredTimeAcc = 0.f;
	_float			m_fConqueredTime = 5.f;

private:
	virtual void My_Tick();
	void	Update_Conquered();

};

END