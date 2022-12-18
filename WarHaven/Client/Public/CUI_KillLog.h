#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CPlayer;

class CUI_KillLog : public CUI_Wrapper
{
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
	void Set_KillLogIndex(_uint iIndex) { m_iCurIndex = iIndex; }
	void Set_LogName(CPlayer* attacker, CPlayer* victim);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void SetActive_KillLog(_bool value);

	void Init_VictimText(wstring Text);
	void Init_AttackerText(wstring Text);

private:
	_uint	m_iCurIndex = 0;
	_float4	m_vStartPosition = _float4(600.f, 330.f, 0.5f, 1.f);
	_float	m_fStepY = 25.f;
	_float	m_fFullSizeX = 0.f;

private:
	_float m_fDisableTime = 5.f;
	_float m_fFadeTime = 0.3f;

	_bool m_bIsDisable = false;

	_float4 vColorRed = _float4(0.75f, 0.2f, 0.2f, 1.f);
	_float4 vColorBlue = _float4(0.15f, 0.5f, 0.6f, 1.f);
	_float4 vColorGreen = _float4(0.f, 0.4f, 0.2f, 1.f);

	_float m_fIconBlank = 20.f;
	_float m_fTextBlank = 15.f;

private:
	enum KillLog { Kill_Icon, Kill_Name, Kill_End };

	CUI_Object* m_pDeadByIcon = nullptr;
	CUI_Object* m_pAttacker[Kill_End];
	CUI_Object* m_pVictim[Kill_End];

private:
	void Create_KillLog();
};

END