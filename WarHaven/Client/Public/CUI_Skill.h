#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUnit;

class CUI_Skill : public CUI_Wrapper
{
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
	void Set_SkillUI(_uint iClass);

	void SetActive_SkillUI(_bool value);
	void SetActive_Outline(_bool value);

	void Enable_AllSkillUI();

private:
	_uint m_iPrvClass = -1;
	_uint m_iCurClass = 0;

private:
	enum SkillUI { SU_BG, SU_Icon, SU_Key, SU_End };

	CUI_Object* m_pSkillUI[SU_End];
	CUI_Object* m_pArrSkillUI[SU_End][3];

	_uint m_iIndex = 0;
	_uint m_iSkillNum = 0;

private:
	void Create_SkillUI();

private:
	enum Outline { Outline0, Outline1, Outline2, Outline_End };
	CUI_Object* m_pOutline[Outline_End];
	CUI_Object* m_pArrOutline[Outline_End][3];

private:
	void Create_Outline();

private:
	CUI_Object* m_pHeroKeySkillIcon = nullptr;

private:
	void Create_HeroKeySkillIcon();
	void Active_HeroKeySkillIcon(_uint eHeroClass);

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	virtual void Set_Shader_SkillGauge1(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_SkillGauge2(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_SkillGauge3(CShader* pShader, const char* pConstName);

public:
	void Set_SkillHUD(_uint iIndex);
	void Set_CoolTime(_uint iSkillType, _float fCoolTime, _float fMaxCoolTime);

private:
	CUI_Object* m_pSkillCoolText = nullptr;
	CUI_Object* m_pSkillCoolTextArr[3];

	CUI_Object* m_pSkillCoolBG = nullptr;
	CUI_Object* m_pSkillCoolBGArr[3];

	_bool m_bAbleOutline = false;

	_uint m_iLerpCount = 0;
	_uint m_iBtnCount = 0;
	_uint m_iPrvSkill = 0;
	_uint m_iCurSkill = 0;

	//_float m_fCoolTime[SkillEnd];
	//_float m_fMaxCoolTime[SkillEnd];
	//_float m_fSkillGauge[SkillEnd];

private:
	virtual void My_Tick() override;

private:
	void Set_Pass();
	void Bind_Shader();

	void Active_SkillHUD(_uint iIndex);
	void Set_SkillBtn(_uint iIndex, _uint iKeyIdx = 52, _uint iIconIdx = 29, bool bRelic = true);

	void Enable_Outline();

private:
	void Create_SkillCoolText();
	void Create_SkillCoolBG();
};

END