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

public:
	void Set_Index(_uint iIndex) { m_iIndex = iIndex; }
	void Set_KillName(wstring wstrKillName);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	_uint m_iIndex = 0;

	_float4	m_vStartPosition = _float4(0.f, -100.f, 0.5f, 1.f);
	_float	m_fStepY = 30.f;

	_bool m_bIsDisable = false;
	_float m_fDisableTime = 3.f;
	_float m_fFadeTime = 0.3f;

	_float4 vColorRed = _float4(0.75f, 0.2f, 0.2f, 1.f);

	_float m_fWhitespace = 5.f;

private:
	enum KillName { Text_Name, Text_Kill, Text_End };
	CUI_Object* m_pKillNameUI[Text_End];

private:
	void Create_KillNameUI();

	void SetActive_KillName(_bool value);
};

END