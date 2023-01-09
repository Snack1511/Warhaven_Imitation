#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Crosshair final : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Crosshair);
	DECLARE_GAMEOBJECT(CUI_Crosshair);

private:
	CUI_Crosshair();
	virtual ~CUI_Crosshair();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_Gauge(CShader* pShader, const char* pConstName);

public:
	void Set_Crosshair(_uint iClass);

	void SetActive_Crosshair(_bool value);
	void SetActive_ArrowUI(_bool value);
	void SetActive_LancerUI(_bool value);

	void Set_Position(_float4 vPos);

	void Set_ArcherPoint(_bool value);

private:
	_uint m_iClassIndex = 0;

private:
	enum CrosshairUI { CU_Point, CU_Outline, CU_End };
	CUI_Object* m_pCrosshair[CU_End];

private:
	void Create_Crosshair();
	void Set_DefaultCrosshair();
	void Set_ArcherCrosshair();

private:
	enum ArrowUI { AU_BG, AU_Arrow, AU_End };
	CUI_Object* m_pArrowUI[AU_End];
	CUI_Object* m_pArrArrowUI[AU_End][3];

	_float4 m_vArrowColor = _float4(1.f, 0.8f, 0.f, 1.f);

	_uint m_iArrowIndex = 0;

	_bool m_bIsCharge = false;
	_bool m_bIsChargeWait = false;

	_uint m_iChargeCount = 0;

private:
	void Create_ArrowUI();
	void Set_ArrowUI();

	void Charge_Arrow();
	void Rotate_Arrow(_float fAngle, _float fDuration);

private:
	enum LancerUI { LU_BG, LU_Gauge, LU_Full, LU_End };
	CUI_Object* m_pLancerUI[LU_End];
	CUI_Object* m_pArrLancerUI[4][LU_End];

private:
	void Create_LancerUI();

private:
	enum GaugeUI { Gauge_BG, Gauge_Bar, Gauge_End };
	CUI_Object* m_pGaugeUI[Gauge_End];

	_float m_fGaugeRatio = 0.f;

private:
	void Create_GaugeUI();

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void Bind_Shader();
};

END