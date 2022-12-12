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
	enum ePOPUP_TYPE {
		eConquest,
		eKILL,
		eKILL2,
		eKILL3,
		eKILL4,
		eCNT
	};

	void Enable_Popup(ePOPUP_TYPE ePopupType);
	void Enable_ConquestPopup(wstring Text, _uint iIconIndex);

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum ConquestPopup{ CP_Line, CP_Blur, CP_BG, CP_ICON, CP_End };
	CUI_Object* m_pConquestPopup[CP_End];

private:
	void Create_ConquestPopup();
};

END