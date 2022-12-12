#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Popup : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Popup);
	DECLARE_GAMEOBJECT(CUI_Popup);

private:
	CUI_Popup();
	virtual ~CUI_Popup();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	void Enable_ConquestPopup(wstring Text);

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum ConquestPopup{ CP_Line, CP_Blur, CP_BG, CP_End };
	CUI_Object* m_pConquestPopup[CP_End];

private:
	void Create_ConquestPopup();
};

END