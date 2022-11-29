#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_LobbyBG final : public CUI
{
	DECLARE_PROTOTYPE(CUI_LobbyBG);
	DECLARE_GAMEOBJECT(CUI_LobbyBG);

private:
	CUI_LobbyBG();
	virtual ~CUI_LobbyBG();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
};

END