#pragma once
#include "CUI_Wrapper.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class CUnit;

class CUI_UnitHUD : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_UnitHUD);
	DECLARE_GAMEOBJECT(CUI_UnitHUD);

	enum UnitUI { UI_Hp, UI_End };

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

public:
	void Set_ProjPos(CTransform* pTransform);

	void Set_UnitStatus(CUnit::UNIT_STATUS tStatus) { m_tStatus = tStatus; }

private:
	CUI_Wrapper* m_pUnitUI[UI_End];

	CUnit::UNIT_STATUS m_tStatus;

private:
	void Ready_UnitHP();
};

END