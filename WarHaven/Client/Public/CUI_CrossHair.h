#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Crosshair final : public CUI_Wrapper
{
	enum WindowType { Point, Outline, ArrowBG, Arrow, GaugeBG, Gauge, Type_End };

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
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void Set_Crosshair(_uint iIndex);

private:
	enum CrosshairUI { CU_Point, CU_Outline, CU_End };
	CUI_Object* m_pCrosshair[CU_End];

private:
	void Create_Crosshair();
	void Init_DefaultCrosshair();

private:
	enum ArrowUI { AU_BG, AU_Arrow, AU_End };
	CUI_Object* m_pArrowUI[AU_End];
	CUI_Object* m_pArrArrowUI[AU_End][3];

	_float4 m_vArrowColor = _float4(1.f, 0.8f, 0.f, 1.f);

private:
	void Create_ArrowUI();
	void Init_ArrowUI(_uint iClass);

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrCrosshair[4][Type_End] = {};

	_bool bIsHero = false;
	_uint m_iPrvCrosshair = 0;
	_uint m_iCurCrosshair = 0;

private:

	void DefaultCrosshair(_uint iIndex = 0);
	void ArrowCrosshair();

private:
	void Ready_Texture();
	void Ready_Crosshair();
};

END

// 레이븐 기본 화살 세개
// 스킬 쓸 때 특수 크로스헤어