#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_Logo : public CUI
{
	DECLARE_PROTOTYPE(CUI_Logo);
	DECLARE_GAMEOBJECT(CUI_Logo);

private:
	CUI_Logo();
	CUI_Logo(const CUI_Logo& Prototype);
	virtual ~CUI_Logo();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void LoadScene(const _uint& event);
};

END