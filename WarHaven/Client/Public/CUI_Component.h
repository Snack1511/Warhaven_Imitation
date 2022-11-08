#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CUI;

class CUI_Component abstract
{
protected:
	CUI_Component();
	virtual ~CUI_Component();

public:
	virtual CUI_Component* Clone() PURE;

public:
	virtual	HRESULT	Initialize_Prototype() PURE;
	virtual	HRESULT	Initialize() PURE;
	virtual void	Start() { OnEnable(); }
	virtual void	Tick() PURE;
	virtual void	Late_Tick() PURE;

public:
	CUI* Set_Subject(CUI* pSubject) { m_pSubject = pSubject; }

protected:
	CUI* m_pSubject = nullptr;

protected:
	virtual	void	OnEnable();
	virtual	void	OnDisable();

	virtual	void	Release() PURE;

private:
	_bool m_bEnable = true;
};

END