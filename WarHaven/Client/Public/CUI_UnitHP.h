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

	void SetActive_UnitHP(_bool value);

private:
	CUI_Object* m_pUnitHP[IT_END];

	_bool m_bIsDecrease = false;
	_float m_fGaugeRatio = 0.f;

private:
	void Bind_Shader();
};

END

// 붉은 게이지가 닳고 일정 시간 뒤에 회색 게이지가 닳는다.