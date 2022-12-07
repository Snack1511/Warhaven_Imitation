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
	void SetActive_SkillCool(_bool value);

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
	enum SkillCool { SC_BG, SC_Text, SC_End };
	CUI_Object* m_pSkillCoolUI[SC_End];
	CUI_Object* m_pArrSkillCoolUI[SC_End][3];

private:
	_bool m_bDisableOutline = false;

	_float m_fOutline1LerpTime = 0.4f;
	_float m_fOutline2LerpTime = 0.5f;

	_float m_fOutline1AccTime[3] = { 0.f };
	_float m_fOutline2AccTime[3] = { 0.f };

private:
	void Create_SkillCoolUI();

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	virtual void Set_Shader_SkillGauge1(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_SkillGauge2(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_SkillGauge3(CShader* pShader, const char* pConstName);

public:
	void Set_CoolTime(_uint iSkillType, _float fCoolTime, _float fMaxCoolTime);

private:
	virtual void My_Tick() override;

private:
	void Set_Pass();
	void Bind_Shader();
};

END