#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUnit;

class CUI_UnitHP : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_UnitHP);
	DECLARE_GAMEOBJECT(CUI_UnitHP);

	enum ImageType { Gauge, BackGauge, BG, IT_END };

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

public:
	virtual void Set_Shader_HpGauge(CShader* pShader, const char* pConstName);

public:
	void Set_ProjPos(CTransform* pTransform);
	void Set_GaugeRatio(_float fRatio) { m_fGaugeRatio = fRatio; }
	void Set_UnitHP(_float fCurHP, _float fMaxHP);
	void Set_UnitHPColor(_float4 vColor) { m_vHpColor = vColor; }

	void SetActive_UnitHP(_bool value);
	void SetActive_HealBlur(_bool value);

private:
	CUI_Object* m_pUnitHP[IT_END];
	CUI_Object* m_pHealBlur = nullptr;

	_bool m_bIsDecrease = false;
	_float m_fGaugeRatio = 0.f;

	_float4 m_vHpColor;

private:
	void Bind_Shader();
};

END