#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUnit;

class CUI_HUD : public CUI_Wrapper
{
	enum HUD_Type{Port, HUD_END};

	DECLARE_PROTOTYPE(CUI_HUD);
	DECLARE_GAMEOBJECT(CUI_HUD);

private:
	CUI_HUD();
	virtual ~CUI_HUD();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick();

private:
	CUI_Wrapper* m_pWrap[HUD_END];

	CUnit::UNIT_STATUS m_tStatus;

	CUnit::CLASS_TYPE m_eCurClass;
	CUnit::CLASS_TYPE m_ePrvClass;

	_bool m_bIsHero = false;

private:
	void Change_UserPort();
	void Set_Portrait(CUnit::CLASS_TYPE eClass);
};

END