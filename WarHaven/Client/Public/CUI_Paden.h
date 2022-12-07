#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Paden : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Paden);
	DECLARE_GAMEOBJECT(CUI_Paden);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	void SetActive_StrongHoldGauge(_bool value);
	void SetActive_TopPointUI(_bool value);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pInGameTimer = nullptr;

	_float m_fInGameTime = 1800.f;

private:
	void Create_InGameTimer();

private:
	enum StrongHoldGauge { Gauge_BG, Gauge_Bar, Gauge_Icon, Gauge_Score, Gauge_End };
	CUI_Object* m_pStrongHoldGauge[Gauge_End];
	CUI_Object* m_pArrStrongHoldGauge[Gauge_End][2];

private:
	void Create_StrongHoldGauge();

private:
	enum TopPointUI { SU_BG, SU_Gauge, SU_Outline, SU_Icon, SU_End };
	CUI_Object* m_pStrongHoldUI[SU_End];
	CUI_Object* m_pArrStrongHoldUI[SU_End][3];

	_float4 m_vColorBG = _float4(0.3f, 0.3f, 0.3f, 1.f);
	_float4 m_vColorGauge = _float4(1.f, 1.f, 1.f, 0.1f);
	_float4 m_vColorOutline = _float4(0.7f, 0.7f, 0.7f, 1.f);

private:
	void Create_StrongHoldUI();
	void Init_StrongHoldUI();
};

END