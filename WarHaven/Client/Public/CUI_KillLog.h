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
	void Set_KillLogType(_uint iKillType);

	void Enable_KillUI(_uint eKillType);

	void MoveUp_KillLog();

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void SetActiv_KillLog(_bool value);
	void SetActiv_KillText(_bool value);

	void Init_VictimText(wstring Text);
	void Init_AttackerText(wstring Text);
	void Init_KillText(wstring Text);

private:
	UI_Type m_eKillType = UI_Type::UT_End;

	_float m_fDisableTime = 2.f;
	_float m_fFadeTime = 0.3f;

	_bool m_bIsDisable = false;

	_float m_fKillLogPosY = 250.f;
	_float m_fKillTextPosY = -100.f;

	_float4 vColorRed = _float4(0.75f, 0.2f, 0.2f, 1.f);
	_float4 vColorBlue = _float4(0.15f, 0.5f, 0.6f, 1.f);
	_float4 vColorGreen = _float4(0.f, 0.4f, 0.2f, 1.f);

	_float4 vDeadByPos;
	_float m_fTextPt = 10.f;
	_float m_fIconBlank = 20.f;
	_float m_fWhitespace = 35.f;

private:
	enum KillLog { Kill_Icon, Kill_Name, Kill_End };

	CUI_Object* m_pDeadByIcon = nullptr;
	CUI_Object* m_pAttacker[Kill_End];
	CUI_Object* m_pVictim[Kill_End];

private:
	enum KillText { Text_Name, Text_Kill, Text_End };
	CUI_Object* m_pKillText[Text_End];

private:
	void Create_KillText();
	void Create_KillLog();
};

END