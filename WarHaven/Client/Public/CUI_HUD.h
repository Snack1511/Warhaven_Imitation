#pragma once
#include "CUI_Wrapper.h"
class CUI_HUD : public CUI_Wrapper
{
	enum UI_TYPE { Play, Barracks, World, Challenge, Profile, Shop, TypeEnd };

	DECLARE_PROTOTYPE(CUI_HUD);
	DECLARE_GAMEOBJECT(CUI_HUD);

private:
	CUI_HUD();
	virtual ~CUI_HUD();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

private:
	CUI_Wrapper* m_Prototypes[TypeEnd] = {};
};

