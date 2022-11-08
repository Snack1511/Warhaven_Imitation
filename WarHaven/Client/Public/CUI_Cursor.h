#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_Cursor final : public CUI
{
	DECLARE_PROTOTYPE(CUI_Cursor);
	DECLARE_GAMEOBJECT(CUI_Cursor);

private:
	CUI_Cursor();
	CUI_Cursor(const CUI_Cursor& Prototype);
	virtual ~CUI_Cursor();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void My_Tick();
};

END