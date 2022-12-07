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
	enum StrongHoldGauge { Gauge_BG, Gauge_Bar, Gauge_Icon, Gauge_End};
	CUI_Object* m_pStrongHoldGauge[Gauge_End];
	CUI_Object* m_pArrStrongHoldGauge[Gauge_End][2];

private:
	void Create_StrongHoldGauge();

private:
	enum TopPointUI { TP_BG, TP_Gauge, TP_Outline, TP_Icon, TP_End };
	CUI_Object* m_pTopPointUI[TP_End];
	CUI_Object* m_pArrTopPointUI[TP_End][3];

private:
	void Create_TopStrongPointUI();
};

END