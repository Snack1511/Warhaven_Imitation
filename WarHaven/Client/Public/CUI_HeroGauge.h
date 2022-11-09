#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_HeroGauge final : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_HeroGauge);
	DECLARE_GAMEOBJECT(CUI_HeroGauge);

private:
	CUI_HeroGauge();
	CUI_HeroGauge(const CUI_HeroGauge& Prototype);
	virtual ~CUI_HeroGauge();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

public:
	void Set_ShaderResources(CShader* pShader, const char* pConstName);

private:
	_float m_fGaugeValue = 1.f;

	_bool m_bHeroAble = false;
	_bool m_bIsHero = false;
};

END