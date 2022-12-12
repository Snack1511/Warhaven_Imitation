#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_HeroGauge final : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_HeroGauge);
	DECLARE_GAMEOBJECT(CUI_HeroGauge);

private:
	CUI_HeroGauge();
	virtual ~CUI_HeroGauge();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_Shader_Gauge(CShader* pShader, const char* pConstName);

public:
	void Set_HeroGauge(_float fCurValue, _float fMaxValue);

	void SetActive_HeroGauge(_bool value);
	void SetActive_AbleHeroText(_bool value);

private:
	enum HeroGauge { HG_BG, HG_Gauge, HG_Text, HG_End };
	CUI_Object* m_pHeroGauge[HG_End];

	CUI_Object* m_pAbleHeroText = nullptr;

	_float m_fCurHeroGauge = 0.f;
	_float m_fMaxHeroGauge = 0.f;
	_float m_fHeroGaugeRatio = 0.f;

	_float m_fValue = 0.f;

private:
	void Create_HeroGauge();
	void Create_AbleHeroText();

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Set_Pass();
	void Bind_Shader();
};

END