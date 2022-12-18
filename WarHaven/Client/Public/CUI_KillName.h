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

	void Enable_KillName(wstring enermyName);

	void Set_OriginPosY();
	void SetActive_KillName(_bool value);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum TextType { Text_Name, Text_Kill, Text_End };
	CUI_Object* m_pKillTextUI[Text_End];

	_float4 vColorRed = _float4(0.75f, 0.2f, 0.2f, 1.f);

	_bool m_bIsDisable = false;

	_float m_fDisableTime = 3.f;
	_float m_fFadeTime = 0.3f;
	_float m_fOriginPosY = -100.f;

private:
	void Create_KillText();
};

END