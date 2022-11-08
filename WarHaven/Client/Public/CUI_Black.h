#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_Black : public CUI
{
	DECLARE_PROTOTYPE(CUI_Black);
	DECLARE_GAMEOBJECT(CUI_Black);

private:
	CUI_Black();
	CUI_Black(const CUI_Black& Prototype);
	virtual ~CUI_Black();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
};

END