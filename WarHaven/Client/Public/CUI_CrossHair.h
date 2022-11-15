#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Crosshair final : public CUI_Wrapper
{
	enum UI_TYPE { Point, Outline, ArrowBG, Arrow, GaugeBG, Gauge, Type_End };

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
	void Set_ShaderResources_Arrow(CShader* pShader, const char* pConstName);
	void Set_ShaderResources_ArrowBG(CShader* pShader, const char* pConstName);

public:
	void Set_Crosshair(_uint iIndex);

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrSkillUI[4][Type_End] = {};

	_bool bIsHero = false;
	_uint m_iPrvCrosshair = 0;
	_uint m_iCurCrosshair = 0;

private:
	virtual void My_Tick() override;

private:
	void Set_Pass();
	void Bind_Shader();

	void DefaultCrosshair(_uint iIndex = 0);
	void ArrowCrosshair();

private:
	void Ready_Texture();
	void Ready_Crosshair();
};

END