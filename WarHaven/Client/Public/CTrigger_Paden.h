#pragma once
#include "CTrigger.h"

BEGIN(Client)

class CPlayer;
class CTeamConnector;
class CDominion_Effect;

class CTrigger_Paden 
	: public CTrigger
{
	
	DECLARE_PROTOTYPE(CTrigger_Paden);

public:
	enum class ePADEN_TRIGGER_TYPE {eSTART, eMAIN, eRESPAWN, eCANNON, eCNT};

protected:
	CTrigger_Paden();
	virtual ~CTrigger_Paden();

public:
	static CTrigger_Paden* Create(string strPositionKey, _float fRadius, ePADEN_TRIGGER_TYPE eEnum);

public:
	virtual void	Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	void		Add_RespawnPositions(_float4 vPosition) { m_vRespawnPositions.push_back(vPosition); };
	void		Set_DominionEffect(CDominion_Effect* pDominion) { m_pDominionEffect = pDominion; }
	CDominion_Effect* Get_DominionEffect() { return m_pDominionEffect; }

public:
	_float4		Get_RespawnPosition();
	string		Get_TriggerName() { return m_strTriggerName; }
	ePADEN_TRIGGER_TYPE	Get_TriggerType() { return m_eTriggerType; }
	void	Set_TriggerType(ePADEN_TRIGGER_TYPE eEnum) { m_eTriggerType = eEnum; }

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Start() override;

private:
	ePADEN_TRIGGER_TYPE	m_eTriggerType = ePADEN_TRIGGER_TYPE::eSTART;
	string				m_strTriggerName;

	eTEAM_TYPE			m_eTeamType = eTEAM_TYPE::eCOUNT;

	/* 양 팀중 몇명이 트리거 안에 들어있는지 확인 */
	_uint				m_iTeamCnt[(_uint)eTEAM_TYPE::eCOUNT] = {};
	//list<CPlayer*>	m_pAdjPlayers[(_uint)eTEAM_TYPE::eCOUNT];

	/* 리스폰 시에 이 포인트에서 front로 생성 */
	list<_float4>		m_vRespawnPositions;

private:
	CTeamConnector*		m_pConqueredTeam = nullptr;

	_float				m_fConqueredTimeAcc = 0.f;
	 _float				m_fConqueredTime = 5.f;
	CDominion_Effect*	m_pDominionEffect = nullptr;

private:
	_bool				m_bUpdateStart = false;

private:
	virtual void My_Tick();
	void	Update_Conquered();

};

END