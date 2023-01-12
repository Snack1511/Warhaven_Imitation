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
	enum ePOPUP_TYPE
	{
		eConquest,
		eKILL, eKILL2, eKILL3, eKILL4,
		eBURGERKING,
		eHEADHUNTER,
		eGLOWKILL,
		eKILLELITE,
		eCNT
	};

	void Enable_Popup(ePOPUP_TYPE ePopupType);
	void Enable_ConquestPopup(wstring Text, _uint iIconIndex);
	void Enable_KillPopup(wstring Text, _uint iIconIndex);

	// 0 �䳢Ż
	void Enable_SkinPopup(_uint iSkin);

	void SetActive_SkinPopup(_bool value);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum ConquestPopup { CP_Line, CP_Blur, CP_BG, CP_ICON, CP_End };
	CUI_Object* m_pConquestPopup[CP_End];

private:
	enum KillPopup { Kill_Icon, Kill_Line, Kill_End };
	CUI_Object* m_pKillPopup[Kill_End];

	_float m_fFadeTime = 0.3f;
	_float m_fEnableTime = 2.f;

private:
	enum SkinPopup
	{
		Skin_PopupBG, Skin_Out, Skin_BG, Skin_Item,
		Skin_Line0, Skin_Line1, Skin_Deco,
		Skin_Text0, Skin_Text1, Skin_Esc,
		Skin_End
	};

	CUI_Object* m_pSKinPopup[Skin_End];

	_bool m_bEnableSkinPopup = false;
	_bool m_bFadePopup = false;

	_uint m_iSkinIdx = 0;

private:
	void Create_ConquestPopup();
	void Create_KillPopup();
	void Create_SkinPopup();
};

END