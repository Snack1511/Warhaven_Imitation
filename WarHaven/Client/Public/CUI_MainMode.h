#pragma once
#include "CUI_Wrapper.h"

class CUI_MainMode : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_MainMode);
	DECLARE_GAMEOBJECT(CUI_MainMode);

private:
	CUI_MainMode();
	virtual ~CUI_MainMode();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void On_PointUpEvent_Start(const _uint& iEventNum);
	virtual void On_PointUpEvent_Mode(const _uint& iEventNum);


private:

};