#pragma once
#include "CUI_Wrapper.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CUnit;

class CUI_Skill : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Skill);
	DECLARE_GAMEOBJECT(CUI_Skill);


	typedef struct tagSkillHud
	{
		enum SKILLHUDNAME { OUTLINE, BG, ICON, KEY, NAME_END };
		CUI_Object* m_pUIInstance[NAME_END] = {};
	}SkillHud;


private:
	CUI_Skill();
	CUI_Skill(const CUI_Skill& Prototype);
	virtual ~CUI_Skill();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void Set_ShaderResources(CShader* pShader, const char* pConstName);
	virtual void Set_ShaderResources_Relic(CShader* pShader, const char* pConstName);

public:
	void Set_SkillUI(_uint iIndex);

private:
	SkillHud tSkillHud;
	SkillHud m_arrSkillHud[4];

	_uint m_iRelicIndex = 0;
	_float m_fRelicValue = 0.f;

private:
	void ActiveSkillBtn(_uint iIndex);
	void Set_SkillIcon(_uint iIndex, _uint iKeyIdx = 52, _uint iIconIdx = 29, bool bRelic = true);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END