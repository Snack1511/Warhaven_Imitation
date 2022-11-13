#pragma once
#include "CUI_Wrapper.h"
class CUI_MainPlay : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_MainPlay);
	DECLARE_GAMEOBJECT(CUI_MainPlay);

private:
	CUI_MainPlay();
	virtual ~CUI_MainPlay();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

private:
	CUI_Object* m_pStartBtn = nullptr;
};

