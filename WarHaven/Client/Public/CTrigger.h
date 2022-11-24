#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

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
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize() override;
	virtual HRESULT	Start() override;

protected:
	COL_GROUP_CLIENT	m_eColGroup = COL_END;
	_float				m_fRadius = 0.f;
	_float4				m_vPosition = ZERO_VECTOR;
};

END