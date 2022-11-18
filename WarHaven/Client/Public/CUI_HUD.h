#pragma once
#include "CUI_Wrapper.h"
#include "CUnit.h"

BEGIN(Client)

class CUnit;

class CUI_HUD : public CUI_Wrapper
{
	enum HUD_Type{Crosshair, Port, Skill, HeroGauge, HUD_END};

	DECLARE_PROTOTYPE(CUI_HUD);
	DECLARE_GAMEOBJECT(CUI_HUD);

private:
	CUI_HUD();
	virtual ~CUI_HUD();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick();

private:
	CUI_Wrapper* m_pWrap[HUD_END];

	CUnit::UNIT_STATUS m_tStatus;

	CUnit::CLASS_TYPE m_eCurClass;
	CUnit::CLASS_TYPE m_ePrvClass;

	_float m_fHeroGauge = 0.f;

	_bool m_bIsEnableHeroPort = false;

private:	// 클래스 변경 창
	CUI_Object* m_pBG = nullptr;
	CUI_Object* m_pPort;
	CUI_Object* m_pPortBG;
	CUI_Object* m_pPortHighlight = nullptr;
	CUI_Object* m_pConfirmBtn = nullptr;

	CUI_Object* m_pPortClone[10];
	CUI_Object* m_pPortBGClone[10];

private:	
	void Set_HUD(CUnit::CLASS_TYPE eClass);
	void Set_Portrait(CUnit::CLASS_TYPE eClass);
	void Set_Crosshair(CUnit::CLASS_TYPE eClass);
	void Set_SkillHUD(CUnit::CLASS_TYPE eClass);

	void Set_ActiveHeroPort(_bool value);

private:
	void Create_CharacterSelectWindow();
};

END