#pragma once
#include "CUI_Wrapper.h"
#include "CUnit.h"

BEGIN(Client)

class CUnit;
class CUI_CharacterWindow;
class CUI_EscMenu;

class CUI_HUD : public CUI_Wrapper
{
public:
	enum HUD { HUD_Crosshair, HUD_Port, HUD_HP, HUD_HeroGauge, HUD_Skill, HUD_End };

	DECLARE_PROTOTYPE(CUI_HUD);
	DECLARE_GAMEOBJECT(CUI_HUD);

private:
	CUI_HUD();
	virtual ~CUI_HUD();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();
	virtual void My_Tick();

public:
	virtual void Set_Shader_HeroTransformGauge(CShader* pShader, const char* pConstName);

	void SetActive_HUD(_bool value);
	void Set_HUD(CLASS_TYPE eClass);

	void SetActive_OxenJumpText(_bool value);
	void SetActive_HeroTransformGauge(_bool value);

	void SetActive_SquardInfo(_bool value);
	void Enable_KillText(wstring text);

public:
	CUI_Wrapper* Get_HUD(_uint eHUD);

private:
	CUI_Wrapper* m_pHUD[HUD_End];

private:
	void Create_HUD();

private:
	CUI_CharacterWindow* m_pUI_CharacterWindow = nullptr;

private:
	void Create_CharacterWindow();
	void Active_CharacterWindow();

private:
	CUI_EscMenu* m_pUI_EscMenu = nullptr;

private:
	void Create_EscMenu();

public:
	_bool Is_OnHeroGauge();

private:
	CUnit::UNIT_STATUS m_tStatus;
	CLASS_TYPE m_eCurClass;
	CLASS_TYPE m_ePrvClass;

private:
	enum SquardInfo { Squard_BG, Squard_Port, Squard_Num, Squard_End };
	CUI_Object* m_pSquardInfo[Squard_End];
	CUI_Object* m_pArrSquardInfo[3][Squard_End];

private:
	void Create_SquardInfo();

private:
	CUI_Object* m_pClassChangeText = nullptr;
	CUI_Object* m_pInactiveHeroText = nullptr;
	CUI_Object* m_pOxenJumpText = nullptr;
	CUI_Object* m_pKillNameText = nullptr;

	_uint m_iKillCount = 0;

private:
	void Create_ClassChangeText();
	void Create_InactiveHeroText();
	void Create_OxenJumpText();
	void Create_KillNameText();

private:
	CUI_Object* m_pPlayerNameText = nullptr;

private:
	void Create_PlayerNameText();

	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_TYPE_CLIENT::LEVEL_END;

private:	// 변신 게이지
	enum HeroTransformUI { HT_BG, HT_Bar, HT_End };
	CUI_Object* m_pHeroTransformUI[HT_End];

	_float m_fMaxHeroTransformValue = 1.44272f;
	_float m_fHeroTransformValue = 0.f;
	_float m_fHeroTransformGaugeRatio = 0.f;

private:
	void Create_HeroTransformUI();

private:
	void Bind_Shader();

	void Update_HeorTransformGauge();
};

END