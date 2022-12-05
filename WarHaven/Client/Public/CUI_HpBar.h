#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_HpBar : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_HpBar);
	DECLARE_GAMEOBJECT(CUI_HpBar);

private:
	CUI_HpBar();
	virtual ~CUI_HpBar();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void SetActive_HP(_bool value);

private:
	enum HPUI { HP_BG, HP_Bar, HP_End };
	CUI_Object* m_pHPUI[HP_End];

private:
	void Create_HPUI();

public:
	void Set_ShaderResourcesBar(CShader* pShader, const char* pConstName);

public:
	void Set_HpRatio(_float value) { m_fHealthRatio = value; }

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Set_Pass();
	void Bind_Shader();

private:
	_float m_fHealthRatio = 0.f;
	_float m_fUVAnimSpeed = 0.f;
};

END