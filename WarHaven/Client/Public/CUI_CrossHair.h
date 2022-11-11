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
	CUI_Crosshair(const CUI_Crosshair& Prototype);
	virtual ~CUI_Crosshair();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrSkillUI[4][Type_End] = {};

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Ready_Texture();
	void Prototype_Disable();
	void Set_Crosshair(_uint iIndex);
	void DefaultCrosshair();
};

END