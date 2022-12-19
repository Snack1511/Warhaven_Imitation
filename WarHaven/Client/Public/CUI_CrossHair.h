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
	void Set_Crosshair(_uint iClass);

	void SetActive_Crosshair(_bool value);
	void SetActive_ArrowUI(_bool value);
	void SetActive_LancerUI(_bool value);

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

private:
	void Create_ArrowUI();
	void Set_ArrowUI();

private:
	enum LancerUI { LU_BG, LU_Gauge, LU_Full, LU_End };
	CUI_Object* m_pLancerUI[LU_End];
	CUI_Object* m_pArrLancerUI[LU_End][4];

private:
	void Create_LancerUI();

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END

// 레이븐 기본 화살 세개
// 스킬 쓸 때 특수 크로스헤어