#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUnit;

class CUI_Skill : public CUI_Wrapper
{
	enum WindowType { Outline0, Outline1, Outline2, BG, Icon, Key, HeroKey, Type_End };

	DECLARE_PROTOTYPE(CUI_Skill);
	DECLARE_GAMEOBJECT(CUI_Skill);

private:
	CUI_Skill();
	virtual ~CUI_Skill();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void Set_ShaderResources_HeroKeySkill(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_SkillGauge(CShader* pShader, const char* pConstName);

public:
	void Set_SkillHUD(_uint iIndex);
	void Set_CoolTime(_uint iSkillType, _float fCoolTime, _float fMaxCoolTime);

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrSkillUI[4][Type_End] = {};

	CUI_Object* m_pSkillCoolText = nullptr;
	CUI_Object* m_pSkillCoolTextArr[3];

	CUI_Object* m_pSkillCoolBG = nullptr;
	CUI_Object* m_pSkillCoolBGArr[3];

	_bool m_bAbleOutline = false;

	_uint m_iLerpCount = 0;
	_uint m_iBtnCount = 0;
	_uint m_iPrvSkill = 0;
	_uint m_iCurSkill = 0;

	_float m_fCoolTime[3];
	_float m_fMaxCoolTime[3];
	_float m_fSkillGauge = 0.f;

private:
	virtual void My_Tick() override;

private:
	void Set_Pass();
	void Bind_Shader();

	void Active_SkillHUD(_uint iIndex);
	void Set_SkillBtn(_uint iIndex, _uint iKeyIdx = 52, _uint iIconIdx = 29, bool bRelic = true);

	void Enable_Outline();

private:
	void Ready_SkillHUD();
	void Create_SkillCoolText();
	void Create_SkillCoolBG();
};

END