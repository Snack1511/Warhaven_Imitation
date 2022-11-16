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

public:
	virtual void On_MouseInEvent(const _uint& iEventNum);
	virtual void On_MouseExitEvent(const _uint& iEventNum);
	virtual void On_ButtonClickEvent(const _uint& iEventNum);

private:
	CUI_Object* m_pStartBtn = nullptr;
};

