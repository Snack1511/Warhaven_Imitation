#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

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
	void Enable_KillUI(UI_Type eKillType);

	void SetActive_KillText(_bool value);
	void SetActive_KillLog(_bool value);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	UI_Type m_eKillType = UI_Type::UT_End;

	_float m_fDisableTime = 2.f;
	_float m_fFadeTime = 0.3f;

	_bool m_bIsDisable = false;

private:	// 오른쪽 상단 킬로그

private:	// 화면 중앙 내가 처치한 적
	CUI_Object* m_pKillText = nullptr;

private:
	void Create_KillText();
	void Create_KillLog();
};

END