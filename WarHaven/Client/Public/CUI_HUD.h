#pragma once
#include "CUI_Wrapper.h"
#include "CUnit.h"

BEGIN(Client)

class CUnit;

class CUI_HUD : public CUI_Wrapper
{
	enum HUD_Type{Crosshair, Port, Skill, HeroGauge, HpBar, HUD_END};

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

public:
	virtual void On_PointEnter_Port(const _uint& iEventNum);
	virtual void On_PointExit_Port(const _uint& iEventNum);
	virtual void On_PointDown_Port(const _uint& iEventNum);

public:
	void SetActive_OxenJumpText(_bool value);
	void SetActive_DamageTex(_float fDmg);

private:
	CUI_Wrapper* m_pWrap[HUD_END];

	CUnit::UNIT_STATUS m_tStatus;
	CUnit::CLASS_TYPE m_eCurClass;
	CUnit::CLASS_TYPE m_ePrvClass;

private:	// 체력바
	_float m_fHpRatio = 0.f;

private:	// 히어로 변신 게이지
	_float m_fHeroGauge = 0.f;
	_bool m_bIsEnableHeroPort = false;

private:	// 클래스 변경 창
	CUI_Object* m_pBG = nullptr;
	CUI_Object* m_pPort;
	CUI_Object* m_pPortBG;
	CUI_Object* m_pClassIcon = nullptr;

	CUI_Object* m_pPortHighlight = nullptr;
	CUI_Object* m_pPortHighlights[6];

	CUI_Object* m_pClassInfo = nullptr;
	CUI_Object* m_pClassInfoIcon = nullptr;
	CUI_Object* m_pLine = nullptr;
	CUI_Object* m_pSelectLine = nullptr;
	CUI_Object* m_pConfirmBtn = nullptr;

	CUI_Object* m_pPortUnderLine = nullptr;
	CUI_Object* m_pPortUnderLines[6];

	CUI_Object* m_pPortClone[6];
	CUI_Object* m_pPortBGClone[6];
	CUI_Object* m_pClassIconClone[6];

	CUI_Object* m_pChangeClassText = nullptr;
	CUI_Object* m_pInactiveHeroText = nullptr;
	CUI_Object* m_pHeroGaugeText = nullptr;
	CUI_Object* m_pOxenJumpText = nullptr;

	CUI_Object* m_pDmgText = nullptr;
	CUI_Object* m_pDmgTexts[32];
	_uint m_iDmgTextIndex = 0;
	_bool m_bDmgTextEffct = false;

private:
	void Bind_Btn();

	void Set_FadePortHighlight();

private:	
	void Set_HUD(CUnit::CLASS_TYPE eClass);
	void Set_Portrait(CUnit::CLASS_TYPE eClass);
	void Set_Crosshair(CUnit::CLASS_TYPE eClass);
	void Set_SkillHUD(CUnit::CLASS_TYPE eClass);

	void Set_ActiveHeroPort(_bool value);

	void SetActive_CharacterSelectWindow(_bool value);
	void Set_ClassInfo(CUnit::CLASS_TYPE eClass);

private:
	void Create_CharacterSelectWindow();
	void Create_PortUnderLine();
	void Create_TraingText();
	void Create_HeroGaugeText();
	void Create_OxenJumpText();
	void Create_DmgText();
};

END