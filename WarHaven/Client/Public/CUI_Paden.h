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
	void Set_Proj_StrongHoldUI(_uint iPointIdx, CTransform* pTransform);

	void SetActive_StrongHoldGauge(_bool value);
	void SetActive_GaugeNum(_bool value);
	void SetActive_TopPointUI(_bool value);

public:
	enum TriggerState { TS_Enter, TS_Exit, TS_End };
	void Interact_StrongHoldUI(string wstrPadenPointKey, _uint iTriggerState);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pInGameTimer = nullptr;

	_float m_fInGameTime = 1800.f;

private:
	void Create_InGameTimer();

private:
	enum StrongHoldGauge { Gauge_BG, Gauge_Bar, Gauge_Icon, Gauge_End };
	CUI_Object* m_pStrongHoldGauge[Gauge_End];
	CUI_Object* m_pArrStrongHoldGauge[Gauge_End][2];

private:
	void Create_StrongHoldGauge();

private:
	enum StrongHoldGaugeNum { Num0, Num1, Num2, Num_End };
	CUI_Object* m_pGaugeNum[Num_End];
	CUI_Object* m_pGauge_Num[2][Num_End];

	_float m_fGaugeNumFadeSpeed = 0.3f;

private:
	void Create_GaugeNum();

private:
	enum TopPointUI { SU_BG, SU_Gauge, SU_Outline, SU_Icon, SU_End };
	CUI_Object* m_pStrongHoldUI[SU_End];
	CUI_Object* m_pArrStrongHoldUI[SU_End][3];

	_float4 m_vColorBG = _float4(0.3f, 0.3f, 0.3f, 1.f);
	_float4 m_vColorGauge = _float4(1.f, 1.f, 1.f, 0.1f);
	_float4 m_vColorOutline = _float4(0.7f, 0.7f, 0.7f, 1.f);

	_float m_fPointUIPosY = 260.f;
	_float m_fMainPointUIPosX = -50.f;

private:
	void Create_StrongHoldUI();
	void Init_StrongHoldUI();

private:
	CUI_Object* m_pProj_StrongHoldUI[3];

private:
	void Create_Proj_StrongHoldUI();
};

END