#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CPlayer;

class CUI_KillLog : public CUI_Wrapper
{
	enum UI_Type { UT_Kill, UT_Log, UT_End };

	DECLARE_PROTOTYPE(CUI_KillLog);
	DECLARE_GAMEOBJECT(CUI_KillLog);

private:
	CUI_KillLog();
	virtual ~CUI_KillLog();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_LogName(CPlayer* attacker, CPlayer* victim);

	void Enable_KillUI(_uint eKillType);

	// void SetActive_KillText(_bool value);
	// void SetActive_KillLog(_bool value);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void SetActiv_KillLog(_bool value);

	void Init_VictimTextPos(wstring Text);
	void Init_AttackerTextPos(wstring Text);

private:
	UI_Type m_eKillType = UI_Type::UT_End;

	_float m_fDisableTime = 2.f;
	_float m_fFadeTime = 0.3f;

	_bool m_bIsDisable = false;

	_float4 vColorRed = _float4(0.75f, 0.2f, 0.2f, 1.f);
	_float4 vColorBlue = _float4(0.15f, 0.5f, 0.6f, 1.f);
	_float4 vColorGreen = _float4(0.f, 0.4f, 0.2f, 1.f);

	_float4 vDeadByPos;
	_float m_fWhitespace = 35.f;

private:
	enum KillLog { Kill_Icon, Kill_Name, Kill_End };

	CUI_Object* m_pDeadByIcon = nullptr;
	CUI_Object* m_pAttacker[Kill_End];
	CUI_Object* m_pVictim[Kill_End];

private:
	CUI_Object* m_pKillText = nullptr;

private:
	void Create_KillText();
	void Create_KillLog();
};

END