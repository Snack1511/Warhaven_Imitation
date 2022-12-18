#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_KillName : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_KillName);
	DECLARE_GAMEOBJECT(CUI_KillName);

private:
	CUI_KillName();
	virtual ~CUI_KillName();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
};

END