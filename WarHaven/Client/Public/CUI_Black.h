#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_Black final : public CUI
{
	DECLARE_PROTOTYPE(CUI_Black);
	DECLARE_GAMEOBJECT(CUI_Black);

private:
	CUI_Black();
	virtual ~CUI_Black();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
};

END