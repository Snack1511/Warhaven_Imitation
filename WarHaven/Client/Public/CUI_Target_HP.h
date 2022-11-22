#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUnit;

class CUI_Target_HP : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Target_HP);
	DECLARE_GAMEOBJECT(CUI_Target_HP);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick() override;
	virtual void My_LateTick() override;;
};

END