#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUnit;

class CUI_Skill : public CUI_Wrapper
{
	enum UI_TYPE { Outline0, Outline1, Outline2, BG, Icon, Key, Type_End };

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
	virtual void Set_ShaderResources_Relic(CShader* pShader, const char* pConstName);

public:
	void Set_SkillHUD(_uint iIndex);

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrSkillUI[4][Type_End] = {};

	_uint m_iRelicIndex = 0;
	_bool m_bIsRelic = false;
	_float m_fRelicValue = 0.f;

	_bool m_bAbleOutline = false;

	_uint m_iBtnCount = 0;

	_bool m_bFirstOutline = false;

private:
	void Set_Pass();
	void Bind_Shader();

	void Enable_SkillHUD();
	void Active_SkillHUD(_uint iIndex);
	void Set_SkillBtn(_uint iIndex, _uint iKeyIdx = 52, _uint iIconIdx = 29, bool bRelic = true);

	void Enable_Outline();

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END