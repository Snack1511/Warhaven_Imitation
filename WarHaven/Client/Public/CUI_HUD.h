#pragma once
#include "CUI_Wrapper.h"
#include "CUnit.h"

BEGIN(Client)

class CUnit;
class CUI_CharacterWindow;

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
	CUI_Wrapper* Get_HUD(_uint eHUD);

public:
	void SetActive_HUD(_bool value);

private:
	CUI_Wrapper* m_pHUD[HUD_End];

private:
	void Create_HUD();

private:
	CUI_CharacterWindow* m_pUI_CharacterWindow = nullptr;

private:
	void Create_CharacterWindow();
	void Active_CharacterWindow();

public:
	virtual void Set_Shader_HeroTransformGauge(CShader* pShader, const char* pConstName);

public:
	void Set_HUD(CLASS_TYPE eClass);

	void SetActive_OxenJumpText(_bool value);
	void SetActive_HeroTransformGauge(_bool value);

public:
	_bool Is_OnHeroGauge();

private:
	CUnit::UNIT_STATUS m_tStatus;
	CLASS_TYPE m_eCurClass;
	CLASS_TYPE m_ePrvClass;

private:
	CUI_Object* m_pOperMapIcon = nullptr;
	CUI_Object* m_pOperMapBG = nullptr;

private:
	void Create_OperMap();

private:	// 작전회의 거점 아이콘
	CUI_Object* m_pTargetPoint = nullptr;
	CUI_Object* m_pArrTargetPoint[2];

private:
	void Create_OperPoint();

private:	// 캐릭터 변경 알림
	CUI_Object* m_pClassChangeText = nullptr;

private:
	void Create_ClassChangeText();

private:	// 히어로 해제 알림
	CUI_Object* m_pInactiveHeroText = nullptr;

private:
	void Create_InactiveHeroText();

private:	// 황소베기 점프 안내
	CUI_Object* m_pOxenJumpText = nullptr;

private:
	void Create_OxenJumpText();

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