#pragma once
#include "CUI_Wrapper.h"
class CUI_Barracks : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Barracks);
	DECLARE_GAMEOBJECT(CUI_Barracks);

private:
	CUI_Barracks();
	virtual ~CUI_Barracks();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

