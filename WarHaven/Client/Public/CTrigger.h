#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPlayer;

class CTrigger abstract
	: public CGameObject
{
protected:
	CTrigger();
	virtual ~CTrigger();

public:
	virtual void	Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	void Add_AdjTriggers(CTrigger* pTrigger) { m_vecAdjTriggers.push_back(pTrigger); }
	void	Reserve_DisableOnStart() { m_bStartDisable = true; }
	_float4 Get_Position() { return m_vPosition; }
public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize() override;
	virtual HRESULT	Start() override;

protected:
	COL_GROUP_CLIENT	m_eColGroup = COL_END;
	_float				m_fRadius = 0.f;
	_float4				m_vPosition = ZERO_VECTOR;

protected:
	vector< CTrigger*> m_vecAdjTriggers;

private:
	_bool		m_bStartDisable = false;
};

END