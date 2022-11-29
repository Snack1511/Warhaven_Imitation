#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_Cursor final : public CUI
{
	DECLARE_PROTOTYPE(CUI_Cursor);
	DECLARE_GAMEOBJECT(CUI_Cursor);

public:
	enum MouseType { Default, Down, Over, Disable, TYPE_END };

private:
	CUI_Cursor();
	virtual ~CUI_Cursor();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick();

public:
	MouseType Get_MouseType() { return m_eType; };
	void Set_Mouse(MouseType eType);

private:
	RECT m_tScreen;
	POINT m_ptMouse;
	_bool m_bIsClick = false;

	MouseType m_eType = MouseType::TYPE_END;

private:
	void SetActive_Mouse(_bool value);
};

END