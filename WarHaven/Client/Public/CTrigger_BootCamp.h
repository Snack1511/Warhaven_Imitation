#pragma once
#include "CTrigger.h"

BEGIN(Client)

class CTrigger_BootCamp final
	: public CTrigger
{
	DECLARE_PROTOTYPE(CTrigger_BootCamp);

private:
	CTrigger_BootCamp();
	virtual ~CTrigger_BootCamp();

public:
	static CTrigger_BootCamp* Create(string strPositionKey, _uint iUIIndex, _float fRadius);

public:
	virtual void	Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);



public:
	virtual HRESULT	Initialize_Prototype() override;

private:
	_uint	m_iUIIndex = 0;

};

END